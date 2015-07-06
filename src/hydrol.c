#include "pihm.h"

int f (realtype t, N_Vector CV_Y, N_Vector CV_Ydot, void *pihm_data)
{
    int             i, j;
    double         *y;
    double         *dy;
    double          dt;
    pihm_struct     pihm;
    elem_struct    *elem;
    river_struct   *riv;

    FILE           *fid;

    y = NV_DATA_S (CV_Y);
    dy = NV_DATA_S (CV_Ydot);
    pihm = (pihm_struct) pihm_data;

    dt = pihm->dt;

    /*
     * Initialization of temporary state variables 
     */
    for (i = 0; i < 3 * pihm->numele + 2 * pihm->numriv; i++)
    {
        dy[i] = 0.0;
    }

    for (i = 0; i < pihm->numele; i++)
    {
        pihm->elem[i].surf = (y[i] >= 0.0) ? y[i] : 0.0;
        pihm->elem[i].unsat = (y[i + pihm->numele] >= 0.0) ? y[i + pihm->numele] : 0.0;
        pihm->elem[i].gw = (y[i + 2 * pihm->numele] >= 0.0) ? y[i + 2 * pihm->numele] : 0.0;
    }

    for (i = 0; i < pihm->numriv; i++)
    {
        pihm->riv[i].stage = (y[i + 3 * pihm->numele] >= 0.0) ? y[i + 3 * pihm->numele] : 0.0;
        pihm->riv[i].gw = (y[i + 3 * pihm->numele + pihm->numriv] >= 0.0) ? y[i + 3 * pihm->numele + pihm->numriv] : 0.0;

        pihm->riv[i].fluxriv[0] = 0.0;
        pihm->riv[i].fluxriv[10] = 0.0;
    }

    LateralFlow (pihm);

    VerticalFlow (pihm);

    RiverFlow (pihm);

    for (i = 0; i < pihm->numele; i++)
    {
        elem = &pihm->elem[i];

        if (elem->gw > elem->soil.depth - elem->soil.dinf)
        {
            dy[i + pihm->numele] += elem->infil - elem->rechg;
#ifdef _NOAH_
            dy[i + 2 * pihm->numele] += elem->rechg - elem->et[2];
#else
            dy[i + 2 * pihm->numele] += elem->rechg - ((elem->surf < EPS / 100.0) ? elem->et[2] : 0.0);
#endif
        }
        else
        {
#ifdef _NOAH_
            dy[i + pihm->numele] += elem->infil - elem->rechg - elem->et[2];
#else
            dy[i + pihm->numele] += elem->infil - elem->rechg - ((elem->surf < EPS / 100.0) ? elem->et[2] : 0.0);
#endif
            dy[i + 2 * pihm->numele] += elem->rechg;
        }

#ifdef _NOAH_
        dy[i] += elem->netprcp - elem->infil;
        dy[i + 2 * pihm->numele] -= elem->et_from_sat * elem->et[1];
        dy[i + pihm->numele] -= (1.0 - elem->et_from_sat) * elem->et[1];
#else
        dy[i] += elem->netprcp - elem->infil - ((elem->surf < EPS / 100.0) ? 0.0 : elem->et[2]);
        if (elem->gw > elem->soil.depth - elem->lc.rzd)
        {
            dy[i + 2 * pihm->numele] -= elem->et[1];
        }
        else
        {
            dy[i + pihm->numele] -= elem->et[1];
        }
#endif

        for (j = 0; j < 3; j++)
        {
            dy[i] -= elem->fluxsurf[j] / elem->topo.area;
            dy[i + 2 * pihm->numele] -= elem->fluxsub[j] / elem->topo.area;
        }

        dy[i + pihm->numele] /= elem->soil.porosity;
        dy[i + 2 * pihm->numele] /= elem->soil.porosity;
        
        if (isnan (dy[i]))
        {
            printf ("ERROR: NAN error for Element %d (surface water) at %lf\n",
                i + 1, t);
            exit (1);
        }
        if (isnan (dy[i + pihm->numele]))
        {
            printf ("ERROR: NAN error for Element %d (unsat water) at %lf\n",
                i + 1, t);
            exit (1);
        }
        if (isnan (dy[i + 2 * pihm->numele]))
        {
            printf ("ERROR: NAN error for Element %d (groundwater) at %lf\n",
                i + 1, t);
            exit (1);
        }
    }

    for (i = 0; i < pihm->numriv; i++)
    {
        riv = &(pihm->riv[i]);

        for (j = 0; j <= 6; j++)
        {
            /* Note the limitation due to
             * d(v)/dt=a*dy/dt+y*da/dt
             * for cs other than rectangle */
            dy[i + 3 * pihm->numele] -= riv->fluxriv[j] / (riv->shp.length * EqWid (riv->shp.intrpl_ord, riv->shp.depth, riv->shp.coeff));
        }
        dy[i + 3 * pihm->numele + pihm->numriv] += -riv->fluxriv[7] - riv->fluxriv[8] - riv->fluxriv[9] - riv->fluxriv[10] + riv->fluxriv[6];
        dy[i + 3 * pihm->numele + pihm->numriv] /= riv->matl.porosity * riv->shp.length * EqWid (riv->shp.intrpl_ord, riv->shp.depth, riv->shp.coeff);

        if (isnan (dy[i + 3 * pihm->numele]))
        {
            printf ("ERROR: NAN error for River Segment %d (river) at %lf\n",
                i + 1, t);
            exit (1);
        }
        if (isnan (dy[i + 3 * pihm->numele + pihm->numriv]))
        {
            printf ("ERROR: NAN error for River Segment %d (groundwater) at %lf\n",
                i + 1, t);
            exit (1);
        }
    }

    //fid = fopen ("dy.txt", "a");

    //fprintf (fid, "tt = %lf\n", t);
    //for (i = 0; i < pihm->numele; i++)
    //{
    //    fprintf (fid, "%lf %d surf %20.17lf\n", t, i, dy[i]);
    //}
    //for (i = 0; i < pihm->numele; i++)
    //{
    //    fprintf (fid, "%lf %d unsat %20.17lf\n", t, i, dy[i + pihm->numele]);
    //}
    //for (i = 0; i < pihm->numele; i++)
    //{
    //    fprintf (fid, "%lf %d GW %20.17lf\n", t, i, dy[i + 2 * pihm->numele]);
    //}
    //for (i = 0; i < 2 * pihm->numriv; i++)
    //{
    //    fprintf (fid, "%lf %d riv %25.22lf\n", t, i, dy[i + 3 * pihm->numele]);
    //}

    //fclose (fid);
    
    return (0);
}

void LateralFlow (pihm_struct pihm)
{
    int             i, j;
    double          dif_y_sub;
    double          avg_y_sub;
    double          distance;
    double          grad_y_sub;
    double          surfh[3];
    double          dhbydx[pihm->numele];
    double          dhbydy[pihm->numele];
    double          effk;
    double          effk_nabr;
    double          avg_ksat;
    double          dif_y_surf;
    double          avg_y_surf;
    double          grad_y_surf;
    double          avg_sf;
    double          avg_rough;
    double          crossa;

    elem_struct    *elem;
    elem_struct    *nabr;
    river_struct   *riv;

    for (i = 0; i < pihm->numele; i++)
    {
        elem = &pihm->elem[i];

        if (pihm->ctrl.surf_mode == 2)
        {
            for (j = 0; j < 3; j++)
            {
                if (elem->nabr[j] > 0)
                {
                    nabr = &pihm->elem[elem->nabr[j] - 1];

                    if (elem->forc.bc_type[j] > -4)
                    {
                        surfh[j] = nabr->topo.zmax + nabr->surf;
                    }
                    else
                    {
                        riv = &pihm->riv[-elem->forc.bc_type[j] / 4 - 1];

                        if (riv->stage > riv->shp.depth)
                        {
                            surfh[j] = riv->topo.zbed + riv->stage;
                        }
                        else
                        {
                            surfh[j] = riv->topo.zmax;
                        }
                    }
                }
                else
                {
                    if (elem->forc.bc_type[j] != 1)
                    {
                        surfh[j] = elem->topo.zmax + elem->surf;
                    }
                    else
                    {
                        surfh[j] = *elem->forc.bc[j];
                    }
                }
            }

            dhbydx[i] = DhByDl (elem->topo.surfy, elem->topo.surfx, surfh);
            dhbydy[i] = DhByDl (elem->topo.surfx, elem->topo.surfy, surfh);
        }
    }

    for (i = 0; i < pihm->numele; i++)
    {
        elem = &pihm->elem[i];

        for (j = 0; j < 3; j++)
        {
            if (elem->nabr[j] > 0)
            {
                nabr = &pihm->elem[elem->nabr[j] - 1];

                /*
                 * Subsurface lateral flux calculation between triangular
                 * elements
                 */
                dif_y_sub = (elem->gw + elem->topo.zmin) - (nabr->gw + nabr->topo.zmin);
                avg_y_sub = AvgY (dif_y_sub, elem->gw, nabr->gw);
                distance = sqrt (pow (elem->topo.x - nabr->topo.x, 2) + pow (elem->topo.y - nabr->topo.y, 2));
                grad_y_sub = dif_y_sub / distance;
                /* Take into account macropore effect */
                effk = EffKH (elem->soil.macropore, elem->gw, elem->soil.depth, elem->soil.dmac, elem->soil.kmach, elem->soil.areafv, elem->soil.ksath);
                effk_nabr = EffKH (nabr->soil.macropore, nabr->gw, nabr->soil.depth, nabr->soil.dmac, nabr->soil.kmach, nabr->soil.areafv, nabr->soil.ksath);
                avg_ksat = 0.5 * (effk + effk_nabr);
                /* Groundwater flow modeled by Darcy's Law */
                elem->fluxsub[j] = avg_ksat * grad_y_sub * avg_y_sub * elem->topo.edge[j];

                /*
                 * Surface lateral flux calculation between triangular
                 * elements
                 */
                if (pihm->ctrl.surf_mode == 1)
                {
                    dif_y_surf = elem->topo.zmax - nabr->topo.zmax;
                }
                else
                {
                    dif_y_surf = (elem->surf + elem->topo.zmax) - (nabr->surf + nabr->topo.zmax);
                }
                avg_y_surf = AvgY (dif_y_surf, elem->surf, nabr->surf);
                grad_y_surf = dif_y_surf / distance;
                avg_sf = 0.5 * (sqrt (pow (dhbydx[i], 2) + pow (dhbydy[i], 2)) + sqrt (pow (dhbydx[elem->nabr[j] - 1], 2) + pow (dhbydy[elem->nabr[j] -1], 2)));
                if (pihm->ctrl.surf_mode == 1)
                {
                    avg_sf = (grad_y_surf > 0.0) ? grad_y_surf : EPS / 1.0E6;
                }
                else
                {
                    avg_sf = (avg_sf > EPS / 1.0E6) ? avg_sf : EPS / 1.0E6;
                }
                /* Weighting needed */
                avg_rough = 0.5 * (elem->lc.rough + nabr->lc.rough);
                crossa = avg_y_surf * elem->topo.edge[j];
                elem->fluxsurf[j] = OverlandFlow (avg_y_surf, grad_y_surf, avg_sf, crossa, avg_rough);
            }
            else    /* Boundary condition flux */
            {
                /* No flow (natural) boundary condition is default */
                if (elem->forc.bc_type[j] == 0)
                {
                    elem->fluxsurf[j] = 0.0;
                    elem->fluxsub[j] = 0.0;
                }
                /* Note: ideally different boundary conditions need to be
                 * incorporated for surf and subsurf respectively */
                else if (elem->forc.bc_type[j] == 1)
                {
                    /* Note: the formulation assumes only Dirichlet TS right
                     * now */
                    /* note the assumption here is no flow for surface */
                    elem->fluxsurf[j] = 0.0;
                    dif_y_sub = elem->gw + elem->topo.zmin - *elem->forc.bc[j];
                    avg_y_sub = AvgY (dif_y_sub, elem->gw, *elem->forc.bc[j] - elem->topo.zmin);
                    /* Minimum distance from circumcenter to the edge of the
                     * triangle on which boundary condition is defined */
                    distance = sqrt (pow (elem->topo.edge[0] * elem->topo.edge[1] * elem->topo.edge[2] / (4.0 * elem->topo.area), 2) - pow (elem->topo.edge[j] / 2.0, 2));
                    effk = EffKH (elem->soil.macropore, elem->gw, elem->soil.depth, elem->soil.dmac, elem->soil.kmach, elem->soil.areafv, elem->soil.ksath);
                    avg_ksat = effk;
                    grad_y_sub = dif_y_sub / distance;
                    elem->fluxsub[j] = avg_ksat * grad_y_sub * avg_y_sub * elem->topo.edge[j];
                }
                else
                {
                    /* Neumann bc (note: md->ele[i].bc[j] value has to be
                     * = 2+(index of neumann boundary ts) */
                    elem->fluxsurf[j] = *elem->forc.bc[j];
                    elem->fluxsub[j] = *elem->forc.bc[j];
                }
            } /* End of specified boundary condition */
        }   /* End of neighbor loop */
    }   /* End of element loop */
}

void VerticalFlow (pihm_struct pihm)
{
    int             i;
    double          grad_y_sub;
    double          satn;
    double          satkfunc;
    double          effk;
    double          dt;
    double          deficit;
    double          avg_y_sub;
    double          total_y;
    double          qmax;
    elem_struct    *elem;

    dt = pihm->dt;

    for (i = 0; i < pihm->numele; i++)
    {
        elem = &pihm->elem[i];

        if (elem->gw > elem->soil.depth - elem->soil.dinf)
        {
            /* Assumption: Dinf < Dmac */
            grad_y_sub = (elem->surf + elem->topo.zmax - (elem->gw + elem->topo.zmin)) / elem->soil.dinf;
            grad_y_sub = (elem->surf < EPS / 100.0 && grad_y_sub > 0.0) ? 0.0 : grad_y_sub;
            satn = 1.0;
            satkfunc = KrFunc (elem->soil.alpha, elem->soil.beta, satn);
            effk = (elem->soil.macropore == 1) ? EffKV (satkfunc, grad_y_sub, SAT_CTRL, elem->soil.kmacv, elem->soil.kinfv, elem->soil.areafh) : elem->soil.kinfv;
#ifdef _NOAH_
            elem->infil = elem->fcr * effk * grad_y_sub;
#else
            elem->infil = effk * grad_y_sub;
#endif

#ifdef _NOAH_
            qmax = elem->surf / dt + elem->netprcp + (elem->fluxsurf[0] + elem->fluxsurf[1] + elem->fluxsurf[2]) / elem->topo.area;
#else
            qmax = elem->surf / dt + elem->netprcp + (elem->fluxsurf[0] + elem->fluxsurf[1] + elem->fluxsurf[2]) / elem->topo.area - ((elem->surf < EPS / 100.0) ? 0.0 : elem->et[2]);
#endif
            elem->infil = (elem->infil < qmax) ? elem->infil : qmax;
            elem->infil = (elem->infil > 0.0) ? elem->infil : 0.0;

            elem->rechg = elem->infil;
        }
        else
        {
            deficit = elem->soil.depth - elem->gw;
#ifdef _NOAH_
            satn = elem->sfcsat;
#else
            satn = elem->unsat / deficit;
#endif
            satn = (satn > 1.0) ? 1.0 : satn;
            satn = (satn < MULTF * EPS) ? (MULTF * EPS) : satn;
            /* note: for psi calculation using van genuchten relation, cutting
             * the psi-sat tail at small saturation can be performed for
             * computational advantage. if you dont' want to perform this,
             * comment the statement that follows */
            avg_y_sub = Psi (satn, elem->soil.alpha, elem->soil.beta);
            avg_y_sub = (avg_y_sub > PSIMIN) ? avg_y_sub : PSIMIN;
            
            total_y = avg_y_sub + elem->topo.zmin + elem->soil.depth - elem->soil.dinf;
            grad_y_sub = (elem->surf + elem->topo.zmax - total_y) / elem->soil.dinf;
            grad_y_sub = (elem->surf < EPS / 100.0 && grad_y_sub > 0.0) ? 0.0 : grad_y_sub;
            satkfunc = KrFunc (elem->soil.alpha, elem->soil.beta, satn);
            if (elem->soil.macropore == 1)
            {
                elem->macpore_status = MacroporeStatus (satkfunc, satn, grad_y_sub, elem->soil.kmacv, elem->soil.kinfv, elem->soil.areafh);
                effk = EffKV (satkfunc, satn, elem->macpore_status, elem->soil.kmacv, elem->soil.kinfv, elem->soil.areafh);
            }
            else
            {
                effk = elem->soil.kinfv;
            }
#ifdef _NOAH_
            elem->infil = elem->fcr * 0.5 * effk * grad_y_sub;
#else
            elem->infil = 0.5 * effk * grad_y_sub;
#endif

#ifdef _NOAH_
            qmax = elem->surf / dt + elem->netprcp + (elem->fluxsurf[0] + elem->fluxsurf[1] + elem->fluxsurf[2]) / elem->topo.area;
#else
            qmax = elem->surf / dt + elem->netprcp + (elem->fluxsurf[0] + elem->fluxsurf[1] + elem->fluxsurf[2]) / elem->topo.area - ((elem->surf < EPS / 100.0) ? 0.0 : elem->et[2]);
#endif
            elem->infil = (elem->infil < qmax) ? elem->infil : qmax;
            elem->infil = (elem->infil > 0.0) ? elem->infil : 0.0;

            /* Harmonic mean formulation.
             * Note that if unsaturated zone has low saturation, satkfunc
             * becomes very small. use arithmetic mean instead */
            //elem->rechg = (satn==0.0)?0:(deficit<=0)?0:(md->ele[i].ksatv*satkfunc*(md->ele[i].alpha*deficit-2*pow(-1+pow(satn,md->ele[i].beta/(-md->ele[i].beta+1)),1/md->ele[i].beta))/(md->ele[i].alpha*((deficit+md->dummyy[i+2*md->numele]*satkfunc))));
            /* Arithmetic mean formulation */
            satn = elem->unsat / deficit;
            satn = (satn > 1.0) ? 1.0 : satn;
            satn = (satn < MULTF * EPS) ? (MULTF * EPS) : satn;
            satkfunc = KrFunc (elem->soil.alpha, elem->soil.beta, satn);
            effk = (elem->soil.macropore == 1 && elem->gw > elem->soil.depth - elem->soil.dmac) ? EffKV (satkfunc, satn, elem->macpore_status, elem->soil.kmacv, elem->soil.ksatv, elem->soil.areafh) : elem->soil.ksatv * satkfunc;

            elem->rechg = (deficit <= 0.0) ? 0.0 : (elem->soil.ksatv * elem->gw + effk * deficit) * (elem->soil.alpha * deficit - 2.0 * pow (-1.0 + pow (satn, elem->soil.beta / (1.0 - elem->soil.beta)), 1.0 / elem->soil.beta)) / (elem->soil.alpha * pow (deficit + elem->gw, 2));
            elem->rechg = (elem->rechg > 0.0 && elem->unsat <= 0.0) ? 0.0 : elem->rechg;
            elem->rechg = (elem->rechg < 0.0 && elem->gw <= 0.0) ? 0.0 : elem->rechg;
        }
    }
}

void RiverFlow (pihm_struct pihm)
{
    river_struct   *riv;
    river_struct   *down;
    elem_struct    *left;
    elem_struct    *right;

    int             i;
    double          total_y;
    double          perim;
    double          total_y_down;
    double          perim_down;
    double          avg_perim;
    double          avg_rough;
    double          distance;
    double          dif_y;
    double          grad_y;
    double          avg_sf;
    double          crossa;
    double          crossa_down;
    double          avg_crossa;
    double          avg_y;
    double          wid;
    double          avg_ksat;
    double          effk_nabr;
    double          effk;
    double          aquifer_depth;
    double          grad_y_sub;
    double          avg_y_sub;
    double          dif_y_sub;
    double          wid_down;
    double          avg_wid;
    double          dt;

    dt = pihm->dt;

    /*
     * Lateral flux calculation between river-river and river-triangular
     * elements
     */
    for (i = 0; i < pihm->numriv; i++)
    {
        riv = &pihm->riv[i];

        total_y = riv->stage + riv->topo.zbed;
        perim = RivPerim (riv->shp.intrpl_ord, riv->stage, riv->shp.coeff);

        if (riv->down > 0)
        {
            down = &pihm->riv[riv->down - 1];

            /* Lateral flux calculation between river-river element */
            total_y_down = down->stage + down->topo.zbed;
            perim_down = RivPerim (down->shp.intrpl_ord, down->stage, down->shp.coeff);
            avg_perim = (perim + perim_down) / 2.0;
            avg_rough = (riv->matl.rough + down->matl.rough) / 2.0;
            distance = 0.5 * (riv->shp.length + down->shp.length);
            dif_y = (pihm->ctrl.riv_mode == 1) ? (riv->topo.zbed - down->topo.zbed) : (total_y - total_y_down);
            grad_y = dif_y / distance;
            avg_sf = (grad_y > 0.0) ? grad_y : EPS;
            crossa = RivArea (riv->shp.intrpl_ord, riv->stage, riv->shp.coeff);
            crossa_down = RivArea (down->shp.intrpl_ord, down->stage, down->shp.coeff);
            avg_crossa = 0.5 * (crossa + crossa_down);
            avg_y = (avg_perim == 0.0) ? 0.0 : (avg_crossa / avg_perim);
            riv->fluxriv[1] = OverlandFlow (avg_y, grad_y, avg_sf, crossa, avg_rough);
            /* Accumulate to get in-flow for down segments:
             * [0] for inflow, [1] for outflow */
            down->fluxriv[0] -= riv->fluxriv[1];

            /* Lateral flux calculation between element beneath river (ebr)
             * and ebr */
            total_y = riv->gw + riv->topo.zmin;
            total_y_down = down->gw + down->topo.zmin;
            wid = EqWid (riv->shp.intrpl_ord, riv->shp.depth, riv->shp.coeff);
            wid_down = EqWid (down->shp.intrpl_ord, down->shp.depth, down->shp.coeff);
            avg_wid = (wid + wid_down) / 2.0;
            distance = 0.5 * (riv->shp.length + down->shp.length);
            dif_y_sub = total_y - total_y_down;
            avg_y_sub = AvgY (dif_y_sub, riv->gw, down->gw);
            grad_y_sub = dif_y_sub / distance;
            /* take care of macropore effect */
            aquifer_depth = riv->topo.zbed - riv->topo.zmin;
            left = &pihm->elem[riv->leftele - 1];
            right = &pihm->elem[riv->rightele - 1];
            effk = 0.5 * (EffKH (left->soil.macropore, left->gw, left->soil.depth, left->soil.dmac, left->soil.kmach, left->soil.areafv, left->soil.ksath)
                + EffKH (right->soil.macropore, right->gw, right->soil.depth, right->soil.dmac, right->soil.kmach, left->soil.areafv, right->soil.ksath));
            left = &pihm->elem[down->leftele - 1];
            right = &pihm->elem[down->rightele - 1];
            effk_nabr = 0.5 * (EffKH (left->soil.macropore, left->gw, left->soil.depth, left->soil.dmac, left->soil.kmach, left->soil.areafv, left->soil.ksath)
                + EffKH (right->soil.macropore, right->gw, right->soil.depth, right->soil.dmac, right->soil.kmach, left->soil.areafv, right->soil.ksath));
            avg_ksat = 0.5 * (effk + effk_nabr);
            /* Groundwater flow modeled by Darcy's law */
            riv->fluxriv[9] = avg_ksat * grad_y_sub * avg_y_sub * avg_wid;
            /* Accumulate to get in-flow for down segments:
             * [10] for inflow, [9] for outflow */
            down->fluxriv[10] -= riv->fluxriv[9];
        }
        else
        {
            switch (riv->down)
            {
                case -1:
                    /* Dirichlet boundary condition */
                    total_y_down = *riv->forc.riverbc + (riv->topo.node_zmax - riv->shp.depth);
                    //distance = 0.5 * riv->shp.length;
                    distance = sqrt (pow (riv->topo.x - pihm->mesh_tbl.x[riv->tonode - 1], 2) + pow (riv->topo.y - pihm->mesh_tbl.y[riv->tonode - 1], 2));
                    grad_y = (total_y - total_y_down) / distance;
                    avg_sf = grad_y;
                    avg_rough = riv->matl.rough;
                    avg_y = AvgY (grad_y, riv->stage, *riv->forc.riverbc);
                    avg_perim = perim;
                    crossa = RivArea (riv->shp.intrpl_ord, riv->stage, riv->shp.coeff);
                    avg_y = (avg_perim == 0.0) ? 0.0 : (crossa / avg_perim);
                    riv->fluxriv[1] = OverlandFlow (avg_y, grad_y, avg_sf, crossa, avg_rough);
                    break;
                case -2:
                    /* Neumann boundary condition */
                    riv->fluxriv[1] = *riv->forc.riverbc;
                    break;
                case -3:
                    /* Zero-depth-gradient boundary conditions */
                    //distance = 0.5 * riv->shp.length;
                    distance = sqrt (pow (riv->topo.x - pihm->mesh_tbl.x[riv->tonode - 1], 2) + pow (riv->topo.y - pihm->mesh_tbl.y[riv->tonode - 1], 2));
                    grad_y = (riv->topo.zbed - (riv->topo.node_zmax - riv->shp.depth)) / distance;
                    avg_rough = riv->matl.rough;
                    avg_y = riv->stage;
                    avg_perim = perim;
                    crossa = RivArea (riv->shp.intrpl_ord, riv->stage, riv->shp.coeff);
                    riv->fluxriv[1] = sqrt (grad_y) * crossa * ((avg_perim > 0.0) ? pow (crossa / avg_perim, 2.0 / 3.0) : 0.0) / avg_rough;
                    break;
                case -4:
                    /* Critical depth boundary conditions */
                    crossa = RivArea (riv->shp.intrpl_ord, riv->stage, riv->shp.coeff);
                    riv->fluxriv[1] = crossa * sqrt (GRAV * riv->stage);
                    break;
                default:
                    printf ("Error: river routing boundary condition type is wrong!\n");
                    exit (1);
            }
            /* Note: boundary condition for subsurface element can be changed.
             * Assumption: no flow condition */
            riv->fluxriv[9] = 0.0;
        }

        left = &pihm->elem[riv->leftele - 1];
        right = &pihm->elem[riv->rightele - 1];

        if (riv->leftele > 0)
        {
            RiverToEle (riv, left, right, LEFT_SIDE, &riv->fluxriv[2], &riv->fluxriv[4], &riv->fluxriv[7], dt);
        }

        if (riv->rightele > 0)
        {
            RiverToEle (riv, right, left, RIGHT_SIDE, &riv->fluxriv[3], &riv->fluxriv[5], &riv->fluxriv[8], dt);
        }

        avg_wid = EqWid (riv->shp.intrpl_ord, riv->stage, riv->shp.coeff);
        if (riv->topo.zbed - (riv->gw + riv->topo.zmin) > 0.0)
        {
            dif_y = riv->stage;
        }
        else
        {
            dif_y = riv->stage + riv->topo.zbed - (riv->gw + riv->topo.zmin);
        }
        grad_y = dif_y / riv->matl.bedthick;
        riv->fluxriv[6] = riv->matl.ksatv * avg_wid * riv->shp.length * grad_y;
    }
}

void RiverToEle (river_struct *riv, elem_struct *elem, elem_struct *oppbank, int side, double *fluxsurf, double *fluxriv, double *fluxsub, double dt)
{
    double          total_y;
    double          dif_y_sub;
    double          avg_y_sub;
    double          effk;
    double          distance;
    double          grad_y_sub;
    double          effk_nabr;
    double          avg_ksat;
    double          aquifer_depth;
    int             j;
    int             replace;

    total_y = riv->stage + riv->topo.zbed;

    /* Lateral surface flux calculation between river-triangular element */
    *fluxsurf = OLFEleToRiv (elem->surf + elem->topo.zmax, elem->topo.zmax, riv->matl.cwr, riv->topo.zmax, total_y, riv->shp.length);

    /* Lateral subsurface flux calculation between river-triangular element */
    dif_y_sub = (riv->stage + riv->topo.zbed) - (elem->gw + elem->topo.zmin);
    /* This is head at river edge representation */
    //avg_y_sub = ((md->riv[i].zmax-(md->ele[md->riv[i].leftele-1].zmax-md->ele[md->riv[i].leftele-1].zmin)+md->dummyy[md->riv[i].leftele-1 + 2*md->numele])>md->riv[i].zmin)?((md->riv[i].zmax-(md->ele[md->riv[i].leftele-1].zmax-md->ele[md->riv[i].leftele-1].zmin)+md->dummyy[md->riv[i].leftele-1 + 2*md->numele])-md->riv[i].zmin):0;
    /* This is head in neighboring cell represention */
    if (elem->topo.zmin > riv->topo.zbed)
    {
        avg_y_sub = elem->gw;
    }
    else if (elem->topo.zmin + elem->gw > riv->topo.zbed)
    {
        avg_y_sub = elem->topo.zmin + elem->gw - riv->topo.zbed;
    }
    else
    {
        avg_y_sub = 0.0;
    }
    avg_y_sub = AvgY (dif_y_sub, riv->stage, avg_y_sub);
    effk = riv->matl.ksath;
    distance = sqrt (pow (riv->topo.x - elem->topo.x, 2) + pow (riv->topo.y - elem->topo.y, 2));
    grad_y_sub = dif_y_sub / distance;
    /* Take into account macropore effect */
    effk_nabr = EffKH (elem->soil.macropore, elem->gw, elem->soil.depth, elem->soil.dmac, elem->soil.kmach, elem->soil.areafv, elem->soil.ksath);
    avg_ksat = 0.5 * (effk + effk_nabr);
    *fluxriv = riv->shp.length * avg_ksat * grad_y_sub * avg_y_sub;

    /* Lateral flux between rectangular element (beneath river) and triangular
     * element */
    dif_y_sub = (riv->gw + riv->topo.zmin) - (elem->gw + elem->topo.zmin);
    /* This is head at river edge representation */
    //avg_y_sub = ((md->riv[i].zmax-(md->ele[md->riv[i].leftele-1].zmax-md->ele[md->riv[i].leftele-1].zmin)+md->dummyy[md->riv[i].leftele-1 + 2*md->numele])>md->riv[i].zmin)?md->riv[i].zmin-(md->riv[i].zmax-(md->ele[md->riv[i].leftele-1].zmax-md->ele[md->riv[i].leftele-1].zmin)):md->dummyy[md->riv[i].leftele-1 + 2*md->numele];
    /* this is head in neighboring cell represention */
    if (elem->topo.zmin > riv->topo.zbed)
    {
        avg_y_sub = 0.0;
    }
    else if (elem->topo.zmin + elem->gw > riv->topo.zbed)
    {
        avg_y_sub = riv->topo.zbed - elem->topo.zmin;
    }
    else
    {
        avg_y_sub = elem->gw;
    }
    avg_y_sub = AvgY (dif_y_sub, riv->gw, avg_y_sub);
    aquifer_depth = riv->topo.zbed - riv->topo.zmin;
    effk = 0.5 * (EffKH (elem->soil.macropore, elem->gw, elem->soil.depth, elem->soil.dmac, elem->soil.kmach, elem->soil.areafv, elem->soil.ksath)
        + EffKH (oppbank->soil.macropore, oppbank->gw, oppbank->soil.depth, oppbank->soil.dmac, oppbank->soil.kmach, oppbank->soil.areafv, oppbank->soil.ksath));
    effk_nabr = EffKH (elem->soil.macropore, elem->gw, elem->soil.depth, elem->soil.dmac, elem->soil.kmach, elem->soil.areafv, elem->soil.ksath);
    avg_ksat = 0.5 * (effk + effk_nabr);
    grad_y_sub = dif_y_sub / distance;
    /* Take into account macropore effect */
    *fluxsub = riv->shp.length * avg_ksat * grad_y_sub * avg_y_sub;

    /* Replace flux term */
    if (side == LEFT_SIDE)
    {
        replace = riv->rightele;
    }
    else
    {
        replace = riv->leftele;
    }
    for (j = 0; j < 3; j++)
    {
        if (elem->nabr[j] == replace)
        {
            if (*fluxsurf < 0.0 && *fluxsurf < elem->fluxsurf[j])
            {
                *fluxsurf = -elem->surf / dt;
            }
            elem->fluxsurf[j] = -(*fluxsurf);
            elem->fluxsub[j] = -(*fluxriv + *fluxsub);
            break;
        }
    }
}