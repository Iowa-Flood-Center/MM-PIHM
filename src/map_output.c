#include "pihm.h"

void MapOutput (char *simulation, pihm_struct pihm, char *outputdir)
{
    int             i, j, k;
    int             n, nT;

    PIHMprintf (VL_VERBOSE, "\nInitializing PIHM output files\n");

    n = 0;
	nT = 0;

    for (i = 0; i < MAXPRINT; i++)
    {
        if (pihm->ctrl.prtvrbl[i] != 0)
        {
            switch (i)
            {
                case SURF_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.surf", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].ws.surf;
                    }
                    n++;
                    break;
                case UNSAT_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.unsat", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].ws.unsat;
                    }
                    n++;
                    break;
                case GW_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.gw", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].ws.gw;
                    }
                    n++;
                    break;
                case RIVSTG_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.stage", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nriver;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nriver; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->riv[j].ws.stage;
                    }
                    n++;
                    break;
                case RIVGW_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.rivgw", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nriver;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nriver; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->riv[j].ws.gw;
                    }
                    n++;
                    break;
                case SNOW_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.snow", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = LS_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].ws.sneqv;
                    }
                    n++;
                    break;
                case CMC_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.is", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = LS_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].ws.cmc;
                    }
                    n++;
                    break;
                case INFIL_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.infil", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].wf.infil;
                    }
                    n++;
                    break;
                case RECHARGE_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.recharge",
                        outputdir, simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].wf.rechg;
                    }
                    n++;
                    break;
                case EC_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.ec", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = LS_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].wf.ec;
                    }
                    n++;
                    break;
                case ETT_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.ett", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = LS_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].wf.ett;
                    }
                    n++;
                    break;
                case EDIR_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.edir", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = LS_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].wf.edir;
                    }
                    n++;
                    break;
                case RIVFLX0_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.rivflx0", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nriver;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nriver; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->riv[j].wf.rivflow[0];
                    }
                    n++;
                    break;
                case RIVFLX1_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.rivflx1", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nriver;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nriver; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->riv[j].wf.rivflow[1];
                    }
                    n++;
                    break;
                case RIVFLX2_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.rivflx2", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nriver;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nriver; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->riv[j].wf.rivflow[2];
                    }
                    n++;
                    break;
                case RIVFLX3_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.rivflx3", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nriver;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nriver; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->riv[j].wf.rivflow[3];
                    }
                    n++;
                    break;
                case RIVFLX4_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.rivflx4", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nriver;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nriver; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->riv[j].wf.rivflow[4];
                    }
                    n++;
                    break;
                case RIVFLX5_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.rivflx5", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nriver;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nriver; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->riv[j].wf.rivflow[5];
                    }
                    n++;
                    break;
                case RIVFLX6_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.rivflx6", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nriver;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nriver; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->riv[j].wf.rivflow[6];
                    }
                    n++;
                    break;
                case RIVFLX7_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.rivflx7", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nriver;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nriver; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->riv[j].wf.rivflow[7];
                    }
                    n++;
                    break;
                case RIVFLX8_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.rivflx8", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nriver;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nriver; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->riv[j].wf.rivflow[8];
                    }
                    n++;
                    break;
                case RIVFLX9_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.rivflx9", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nriver;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nriver; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->riv[j].wf.rivflow[9];
                    }
                    n++;
                    break;
                case RIVFLX10_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.rivflx10",
                        outputdir, simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nriver;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nriver; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->riv[j].wf.rivflow[10];
                    }
                    n++;
                    break;
                case SUBFLX_CTRL:
                    for (k = 0; k < 3; k++)
                    {
                        sprintf (pihm->prtctrl[n].name, "%s%s.subflx%d",
                            outputdir, simulation, k);
                        pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                        pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                        pihm->prtctrl[n].nvar = nelem;
                        pihm->prtctrl[n].var =
                            (double **)malloc (pihm->prtctrl[n].nvar *
                            sizeof (double *));
                        for (j = 0; j < nelem; j++)
                        {
                            pihm->prtctrl[n].var[j] =
                                &pihm->elem[j].wf.subsurf[k];
                        }
                        n++;
                    }
                    break;
                case SURFFLX_CTRL:
                    for (k = 0; k < 3; k++)
                    {
                        sprintf (pihm->prtctrl[n].name, "%s%s.surfflx%d",
                            outputdir, simulation, k);
                        pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                        pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                        pihm->prtctrl[n].nvar = nelem;
                        pihm->prtctrl[n].var =
                            (double **)malloc (pihm->prtctrl[n].nvar *
                            sizeof (double *));
                        for (j = 0; j < nelem; j++)
                        {
                            pihm->prtctrl[n].var[j] =
                                &pihm->elem[j].wf.ovlflow[k];
                        }
                        n++;
                    }
                    break;
#ifdef _NOAH_
                case T1_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.t1", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = LS_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].es.t1;
                    }
                    n++;
                    break;
                case STC_CTRL:
                    for (k = 0; k < MAXLYR; k++)
                    {
                        sprintf (pihm->prtctrl[n].name, "%s%s.stc%d",
                            outputdir, simulation, k);
                        pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                        pihm->prtctrl[n].upd_intvl = LS_STEP;
                        pihm->prtctrl[n].nvar = nelem;
                        pihm->prtctrl[n].var =
                            (double **)malloc (pihm->prtctrl[n].nvar *
                            sizeof (double *));
                        for (j = 0; j < nelem; j++)
                        {
                            pihm->prtctrl[n].var[j] =
                                &pihm->elem[j].es.stc[k];
                        }
                        n++;
                    }
                    break;
                case SMC_CTRL:
                    for (k = 0; k < MAXLYR; k++)
                    {
                        sprintf (pihm->prtctrl[n].name, "%s%s.smc%d",
                            outputdir, simulation, k);
                        pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                        pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                        pihm->prtctrl[n].nvar = nelem;
                        pihm->prtctrl[n].var =
                            (double **)malloc (pihm->prtctrl[n].nvar *
                            sizeof (double *));
                        for (j = 0; j < nelem; j++)
                        {
                            pihm->prtctrl[n].var[j] =
                                &pihm->elem[j].ws.smc[k];
                        }
                        n++;
                    }
                    break;
                case SH2O_CTRL:
                    for (k = 0; k < MAXLYR; k++)
                    {
                        sprintf (pihm->prtctrl[n].name, "%s%s.swc%d",
                            outputdir, simulation, k);
                        pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                        pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                        pihm->prtctrl[n].nvar = nelem;
                        pihm->prtctrl[n].var =
                            (double **)malloc (pihm->prtctrl[n].nvar *
                            sizeof (double *));
                        for (j = 0; j < nelem; j++)
                        {
                            pihm->prtctrl[n].var[j] =
                                &pihm->elem[j].ws.sh2o[k];
                        }
                        n++;
                    }
                    break;
                case SNOWH_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.snowh", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = LS_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].ps.snowh;
                    }
                    n++;
                    break;
                case ALBEDO_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.albedo", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = LS_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].ps.albedo;
                    }
                    n++;
                    break;
                case LE_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.le", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = LS_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].ef.eta;
                    }
                    n++;
                    break;
                case SH_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.sh", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = LS_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].ef.sheat;
                    }
                    n++;
                    break;
                case G_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.g", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = LS_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].ef.ssoil;
                    }
                    n++;
                    break;
                case ETP_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.etp", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = LS_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].ef.etp;
                    }
                    n++;
                    break;
                case ESNOW_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.esnow", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = LS_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].ef.esnow;
                    }
                    n++;
                    break;
                case ROOTW_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.rootw", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = LS_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].ps.soilw;
                    }
                    n++;
                    break;
                case SOILM_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.soilm", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = LS_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].ws.soilm;
                    }
                    n++;
                    break;
                case SOLAR_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.solar", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = LS_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].ef.soldn;
                    }
                    n++;
                    break;
                case CH_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.ch", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = LS_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].ps.ch;
                    }
                    n++;
                    break;
#endif

#ifdef _BGC_
                case LAI_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.lai", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = CN_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].ps.proj_lai;
                    }
                    n++;
                    break;
                case VEGC_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.vegc", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = CN_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->elem[j].summary.vegc;
                    }
                    n++;
                    break;
                case LITRC_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.litrc", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = CN_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->elem[j].summary.litrc;
                    }
                    n++;
                    break;
                case SOILC_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.soilc", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = CN_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->elem[j].summary.soilc;
                    }
                    n++;
                    break;
                case TOTALC_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.totalc", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = CN_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->elem[j].summary.totalc;
                    }
                    n++;
                    break;
                case NPP_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.npp", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = CN_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->elem[j].summary.daily_npp;
                    }
                    n++;
                    break;
                case NEP_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.nep", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = CN_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->elem[j].summary.daily_nep;
                    }
                    n++;
                    break;
                case NEE_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.nee", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = CN_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->elem[j].summary.daily_nee;
                    }
                    n++;
                    break;
                case GPP_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.gpp", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = CN_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->elem[j].summary.daily_gpp;
                    }
                    n++;
                    break;
                case SMINN_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.sminn", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].ns.sminn;
                    }
                    n++;
                    break;
#endif
#ifdef _CYCLES_
                case BIOMASS_CTRL:
                    for (k = 0; k < pihm->elem[0].comm.NumCrop; k++)
                    {
                        sprintf (pihm->prtctrl[n].name, "%s%s.%s.biomass",
                            outputdir, simulation,
                            pihm->elem[0].comm.Crop[k].cropName);
                        pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                        pihm->prtctrl[n].upd_intvl = CN_STEP;
                        pihm->prtctrl[n].nvar = nelem;
                        pihm->prtctrl[n].var =
                            (double **)malloc (pihm->prtctrl[n].nvar *
                            sizeof (double *));
                        for (j = 0; j < nelem; j++)
                        {
                            pihm->prtctrl[n].var[j] =
                                &pihm->elem[j].comm.Crop[k].svBiomass;
                        }
                        n++;
                    }

                    sprintf (pihm->prtctrl[n].name, "%s%s.comm.biomass",
                        outputdir, simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = CN_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->elem[j].comm.svBiomass;
                    }
                    n++;
                    break;
                case RADNINTCP_CTRL:
                    for (k = 0; k < pihm->elem[0].comm.NumCrop; k++)
                    {
                        sprintf (pihm->prtctrl[n].name, "%s%s.%s.radintcp",
                            outputdir, simulation,
                            pihm->elem[0].comm.Crop[k].cropName);
                        pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                        pihm->prtctrl[n].upd_intvl = CN_STEP;
                        pihm->prtctrl[n].nvar = nelem;
                        pihm->prtctrl[n].var =
                            (double **)malloc (pihm->prtctrl[n].nvar *
                            sizeof (double *));
                        for (j = 0; j < nelem; j++)
                        {
                            pihm->prtctrl[n].var[j] =
                                &pihm->
                                elem[j].comm.Crop[k].svRadiationInterception;
                        }
                        n++;
                    }

                    sprintf (pihm->prtctrl[n].name, "%s%s.comm.radintcp",
                        outputdir, simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = CN_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->elem[j].comm.svRadiationInterception;
                    }
                    n++;
                    break;
                case WATER_STS_CTRL:
                    for (k = 0; k < pihm->elem[0].comm.NumCrop; k++)
                    {
                        sprintf (pihm->prtctrl[n].name, "%s%s.%s.waterstress",
                            outputdir, simulation,
                            pihm->elem[0].comm.Crop[k].cropName);
                        pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                        pihm->prtctrl[n].upd_intvl = CN_STEP;
                        pihm->prtctrl[n].nvar = nelem;
                        pihm->prtctrl[n].var =
                            (double **)malloc (pihm->prtctrl[n].nvar *
                            sizeof (double *));
                        for (j = 0; j < nelem; j++)
                        {
                            pihm->prtctrl[n].var[j] =
                                &pihm->
                                elem[j].comm.Crop[k].svWaterStressFactor;
                        }
                        n++;
                    }

                    sprintf (pihm->prtctrl[n].name, "%s%s.comm.waterstress",
                        outputdir, simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = CN_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->elem[j].comm.svWaterStressFactor;
                    }
                    n++;
                    break;
                case N_STS_CTRL:
                    for (k = 0; k < pihm->elem[0].comm.NumCrop; k++)
                    {
                        sprintf (pihm->prtctrl[n].name, "%s%s.%s.nstress",
                            outputdir, simulation,
                            pihm->elem[0].comm.Crop[k].cropName);
                        pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                        pihm->prtctrl[n].upd_intvl = CN_STEP;
                        pihm->prtctrl[n].nvar = nelem;
                        pihm->prtctrl[n].var =
                            (double **)malloc (pihm->prtctrl[n].nvar *
                            sizeof (double *));
                        for (j = 0; j < nelem; j++)
                        {
                            pihm->prtctrl[n].var[j] =
                                &pihm->elem[j].comm.Crop[k].svN_StressFactor;
                        }
                        n++;
                    }

                    sprintf (pihm->prtctrl[n].name, "%s%s.comm.nstress",
                        outputdir, simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = CN_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->elem[j].comm.svN_StressFactor;
                    }
                    n++;
                    break;
                case CROP_TR_CTRL:
                    for (k = 0; k < pihm->elem[0].comm.NumCrop; k++)
                    {
                        sprintf (pihm->prtctrl[n].name, "%s%s.%s.transp",
                            outputdir, simulation,
                            pihm->elem[0].comm.Crop[k].cropName);
                        pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                        pihm->prtctrl[n].upd_intvl = LS_STEP;
                        pihm->prtctrl[n].nvar = nelem;
                        pihm->prtctrl[n].var =
                            (double **)malloc (pihm->prtctrl[n].nvar *
                            sizeof (double *));
                        for (j = 0; j < nelem; j++)
                        {
                            pihm->prtctrl[n].var[j] =
                                &pihm->elem[j].comm.Crop[k].svTranspiration;
                        }
                        n++;
                    }

                    sprintf (pihm->prtctrl[n].name, "%s%s.comm.transp",
                        outputdir, simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = LS_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->elem[j].comm.svTranspiration;
                    }
                    n++;
                    break;
                case CROP_POTTR_CTRL:
                    for (k = 0; k < pihm->elem[0].comm.NumCrop; k++)
                    {
                        sprintf (pihm->prtctrl[n].name, "%s%s.%s.pottransp",
                            outputdir, simulation,
                            pihm->elem[0].comm.Crop[k].cropName);
                        pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                        pihm->prtctrl[n].upd_intvl = LS_STEP;
                        pihm->prtctrl[n].nvar = nelem;
                        pihm->prtctrl[n].var =
                            (double **)malloc (pihm->prtctrl[n].nvar *
                            sizeof (double *));
                        for (j = 0; j < nelem; j++)
                        {
                            pihm->prtctrl[n].var[j] =
                                &pihm->
                                elem[j].comm.Crop[k].svTranspirationPotential;
                        }
                        n++;
                    }

                    sprintf (pihm->prtctrl[n].name, "%s%s.comm.pottransp",
                        outputdir, simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = LS_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->elem[j].comm.svTranspirationPotential;
                    }
                    n++;
                    break;
                case RES_EVAP_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.eres", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = LS_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].wf.eres;
                    }
                    n++;
                    break;
                case NO3_PROF_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.NO3", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->elem[j].soil.NO3Profile;
                    }
                    n++;
                    break;
                case NO3_RIVER_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.rivNO3", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nriver;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nriver; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->riv[j].NO3sol.soluteMass;
                    }
                    n++;
                    break;
                case NH4_PROF_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.NH4", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->elem[j].soil.NH4Profile;
                    }
                    n++;
                    break;
                case NH4_RIVER_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.rivNH4", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                    pihm->prtctrl[n].nvar = nriver;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nriver; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->riv[j].NH4sol.soluteMass;
                    }
                    n++;
                    break;
                case NO3_DENIT_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.NO3denitrif",
                        outputdir, simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = CN_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] =
                            &pihm->elem[j].soil.NO3_Denitrification;
                    }
                    n++;
                    break;
                case NO3_LEACH_CTRL:
                    for (k = 0; k < NUM_EDGE; k++)
                    {
                        sprintf (pihm->prtctrl[n].name, "%s%s.NO3leaching%d",
                            outputdir, simulation, k);
                        pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                        pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                        pihm->prtctrl[n].nvar = nelem;
                        pihm->prtctrl[n].var =
                            (double **)malloc (pihm->prtctrl[n].nvar *
                            sizeof (double *));
                        for (j = 0; j < nelem; j++)
                        {
                            pihm->prtctrl[n].var[j] =
                                &pihm->elem[j].soil.NO3Leaching[k];
                        }
                        n++;
                    }
                    break;
                case NH4_LEACH_CTRL:
                    for (k = 0; k < NUM_EDGE; k++)
                    {
                        sprintf (pihm->prtctrl[n].name, "%s%s.NH4leaching%d",
                            outputdir, simulation, k);
                        pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                        pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                        pihm->prtctrl[n].nvar = nelem;
                        pihm->prtctrl[n].var =
                            (double **)malloc (pihm->prtctrl[n].nvar *
                            sizeof (double *));
                        for (j = 0; j < nelem; j++)
                        {
                            pihm->prtctrl[n].var[j] =
                                &pihm->elem[j].soil.NH4Leaching[k];
                        }
                        n++;
                    }
                    break;
                case NO3_LEACH_RIVER_CTRL:
                    for (k = 0; k < 4; k++)
                    {
                        sprintf (pihm->prtctrl[n].name,
                            "%s%s.riverNO3leaching%d",
                            outputdir, simulation, k);
                        pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                        pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                        pihm->prtctrl[n].nvar = nriver;
                        pihm->prtctrl[n].var =
                            (double **)malloc (pihm->prtctrl[n].nvar *
                            sizeof (double *));
                        for (j = 0; j < nriver; j++)
                        {
                            pihm->prtctrl[n].var[j] =
                                &pihm->riv[j].NO3Leaching[k];
                        }
                        n++;
                    }
                    break;
                case NH4_LEACH_RIVER_CTRL:
                    for (k = 0; k < 4; k++)
                    {
                        sprintf (pihm->prtctrl[n].name,
                            "%s%s.riverNH4leaching%d",
                            outputdir, simulation, k);
                        pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                        pihm->prtctrl[n].upd_intvl = HYDROL_STEP;
                        pihm->prtctrl[n].nvar = nriver;
                        pihm->prtctrl[n].var =
                            (double **)malloc (pihm->prtctrl[n].nvar *
                            sizeof (double *));
                        for (j = 0; j < nriver; j++)
                        {
                            pihm->prtctrl[n].var[j] =
                                &pihm->riv[j].NH4Leaching[k];
                        }
                        n++;
                    }
                    break;
                case LAI_CTRL:
                    sprintf (pihm->prtctrl[n].name, "%s%s.lai", outputdir,
                        simulation);
                    pihm->prtctrl[n].intvl = pihm->ctrl.prtvrbl[i];
                    pihm->prtctrl[n].upd_intvl = CN_STEP;
                    pihm->prtctrl[n].nvar = nelem;
                    pihm->prtctrl[n].var =
                        (double **)malloc (pihm->prtctrl[n].nvar *
                        sizeof (double *));
                    for (j = 0; j < nelem; j++)
                    {
                        pihm->prtctrl[n].var[j] = &pihm->elem[j].ps.proj_lai;
                    }
                    n++;
                    break;

#endif
				case SURFTEC_CTRL:
					sprintf(pihm->prtctrlT[nT].name, "%s%s_surf", outputdir,
						simulation);
					pihm->prtctrlT[nT].intvl = pihm->ctrl.prtvrbl[i];
					pihm->prtctrlT[nT].intr = 0;
					pihm->prtctrlT[nT].nvar = nelem;
					pihm->prtctrlT[nT].nnodes = pihm->meshtbl.numnode;

					pihm->prtctrlT[nT].x =
						(double **)malloc(pihm->prtctrlT[nT].nnodes *
							sizeof(double *));
					pihm->prtctrlT[nT].y =
						(double **)malloc(pihm->prtctrlT[nT].nnodes *
							sizeof(double *));
					pihm->prtctrlT[nT].zmin =
						(double **)malloc(pihm->prtctrlT[nT].nnodes *
							sizeof(double *));
					pihm->prtctrlT[nT].zmax =
						(double **)malloc(pihm->prtctrlT[nT].nnodes *
							sizeof(double *));
					for (j = 0; j < pihm->prtctrlT[nT].nnodes; j++)
					{
						pihm->prtctrlT[nT].x[j] = &pihm->meshtbl.x[j];
						pihm->prtctrlT[nT].y[j] = &pihm->meshtbl.y[j];
						pihm->prtctrlT[nT].zmax[j] = &pihm->meshtbl.zmax[j];
						pihm->prtctrlT[nT].zmin[j] = &pihm->meshtbl.zmin[j];

					}
					pihm->prtctrlT[nT].node0 =
						(int **)malloc(pihm->prtctrlT[nT].nvar *
							sizeof(int *));
					pihm->prtctrlT[nT].node1 =
						(int **)malloc(pihm->prtctrlT[nT].nvar *
							sizeof(int *));
					pihm->prtctrlT[nT].node2 =
						(int **)malloc(pihm->prtctrlT[nT].nvar *
							sizeof(int *));
					pihm->prtctrlT[nT].var =
						(double **)malloc(pihm->prtctrlT[nT].nvar *
							sizeof(double *));

					for (j = 0; j < nelem; j++)
					{
						pihm->prtctrlT[nT].var[j] = &pihm->elem[j].ws.surf;
						pihm->prtctrlT[nT].node0[j] = &pihm->elem[j].node[0];
						pihm->prtctrlT[nT].node1[j] = &pihm->elem[j].node[1];
						pihm->prtctrlT[nT].node2[j] = &pihm->elem[j].node[2];
					}
					nT++;
					break;
				case UNSATTEC_CTRL:
					sprintf(pihm->prtctrlT[nT].name, "%s%s_unsat", outputdir,
						simulation);
					pihm->prtctrlT[nT].intvl = pihm->ctrl.prtvrbl[i];
					pihm->prtctrlT[nT].intr = 0;
					pihm->prtctrlT[nT].nvar = nelem;
					pihm->prtctrlT[nT].nnodes = pihm->meshtbl.numnode;

					pihm->prtctrlT[nT].x =
						(double **)malloc(pihm->prtctrlT[nT].nnodes *
							sizeof(double *));
					pihm->prtctrlT[nT].y =
						(double **)malloc(pihm->prtctrlT[nT].nnodes *
							sizeof(double *));
					pihm->prtctrlT[nT].zmin =
						(double **)malloc(pihm->prtctrlT[nT].nnodes *
							sizeof(double *));
					pihm->prtctrlT[nT].zmax =
						(double **)malloc(pihm->prtctrlT[nT].nnodes *
							sizeof(double *));
					for (j = 0; j < pihm->prtctrlT[nT].nnodes; j++)
					{
						pihm->prtctrlT[nT].x[j] = &pihm->meshtbl.x[j];
						pihm->prtctrlT[nT].y[j] = &pihm->meshtbl.y[j];
						pihm->prtctrlT[nT].zmax[j] = &pihm->meshtbl.zmax[j];
						pihm->prtctrlT[nT].zmin[j] = &pihm->meshtbl.zmin[j];

					}
					pihm->prtctrlT[nT].node0 =
						(int **)malloc(pihm->prtctrlT[nT].nvar *
							sizeof(int *));
					pihm->prtctrlT[nT].node1 =
						(int **)malloc(pihm->prtctrlT[nT].nvar *
							sizeof(int *));
					pihm->prtctrlT[nT].node2 =
						(int **)malloc(pihm->prtctrlT[nT].nvar *
							sizeof(int *));
					pihm->prtctrlT[nT].var =
						(double **)malloc(pihm->prtctrlT[nT].nvar *
							sizeof(double *));

					for (j = 0; j < nelem; j++)
					{
						pihm->prtctrlT[nT].var[j] = &pihm->elem[j].ws.unsat;
						pihm->prtctrlT[nT].node0[j] = &pihm->elem[j].node[0];
						pihm->prtctrlT[nT].node1[j] = &pihm->elem[j].node[1];
						pihm->prtctrlT[nT].node2[j] = &pihm->elem[j].node[2];
					}
					nT++;
					break;
				case GWTEC_CTRL:
					sprintf(pihm->prtctrlT[nT].name, "%s%s_gw", outputdir,
						simulation);
					pihm->prtctrlT[nT].intvl = pihm->ctrl.prtvrbl[i];
					pihm->prtctrlT[nT].intr = 0;
					pihm->prtctrlT[nT].nvar = nelem;
					pihm->prtctrlT[nT].nnodes = pihm->meshtbl.numnode;

					pihm->prtctrlT[nT].x =
						(double **)malloc(pihm->prtctrlT[nT].nnodes *
							sizeof(double *));
					pihm->prtctrlT[nT].y =
						(double **)malloc(pihm->prtctrlT[nT].nnodes *
							sizeof(double *));
					pihm->prtctrlT[nT].zmin =
						(double **)malloc(pihm->prtctrlT[nT].nnodes *
							sizeof(double *));
					pihm->prtctrlT[nT].zmax =
						(double **)malloc(pihm->prtctrlT[nT].nnodes *
							sizeof(double *));
					for (j = 0; j < pihm->prtctrlT[nT].nnodes; j++)
					{
						pihm->prtctrlT[nT].x[j] = &pihm->meshtbl.x[j];
						pihm->prtctrlT[nT].y[j] = &pihm->meshtbl.y[j];
						pihm->prtctrlT[nT].zmax[j] = &pihm->meshtbl.zmax[j];
						pihm->prtctrlT[nT].zmin[j] = &pihm->meshtbl.zmin[j];

					}
					pihm->prtctrlT[nT].node0 =
						(int **)malloc(pihm->prtctrlT[nT].nvar *
							sizeof(int *));
					pihm->prtctrlT[nT].node1 =
						(int **)malloc(pihm->prtctrlT[nT].nvar *
							sizeof(int *));
					pihm->prtctrlT[nT].node2 =
						(int **)malloc(pihm->prtctrlT[nT].nvar *
							sizeof(int *));
					pihm->prtctrlT[nT].var =
						(double **)malloc(pihm->prtctrlT[nT].nvar *
							sizeof(double *));

					for (j = 0; j < nelem; j++)
					{
						pihm->prtctrlT[nT].var[j] = &pihm->elem[j].ws.gw;
						pihm->prtctrlT[nT].node0[j] = &pihm->elem[j].node[0];
						pihm->prtctrlT[nT].node1[j] = &pihm->elem[j].node[1];
						pihm->prtctrlT[nT].node2[j] = &pihm->elem[j].node[2];
					}
					nT++;
					break;
				case RIVSTGTEC_CTRL:
					sprintf(pihm->prtctrlT[nT].name, "%s%s_stage", outputdir,
						simulation);
					pihm->prtctrlT[nT].intvl = pihm->ctrl.prtvrbl[i];
					pihm->prtctrlT[nT].intr = 1;
					pihm->prtctrlT[nT].nvar = nriver;
					pihm->prtctrlT[nT].var =
						(double **)malloc(pihm->prtctrlT[nT].nvar *
							sizeof(double *));
					pihm->prtctrlT[nT].x =
						(double **)malloc(pihm->prtctrlT[nT].nvar *
							sizeof(double *));
					pihm->prtctrlT[nT].y =
						(double **)malloc(pihm->prtctrlT[nT].nvar *
							sizeof(double *));
					pihm->prtctrlT[nT].zmin =
						(double **)malloc(pihm->prtctrlT[nT].nvar *
							sizeof(double *));
					pihm->prtctrlT[nT].zmax =
						(double **)malloc(pihm->prtctrlT[nT].nvar *
							sizeof(double *));
					for (j = 0; j < nriver; j++)
					{
						pihm->prtctrlT[nT].var[j] = &pihm->riv[j].ws.stage;
						pihm->prtctrlT[nT].x[j] = &pihm->riv[j].topo.x;
						pihm->prtctrlT[nT].y[j] = &pihm->riv[j].topo.y;
						pihm->prtctrlT[nT].zmax[j] = &pihm->riv[j].topo.zmax;
						pihm->prtctrlT[nT].zmin[j] = &pihm->riv[j].topo.zmin;
					}
					nT++;
					break;
				case RIVGWTEC_CTRL:
					sprintf(pihm->prtctrlT[nT].name, "%s%s_rivgw", outputdir,
						simulation);
					pihm->prtctrlT[nT].intvl = pihm->ctrl.prtvrbl[i];
					pihm->prtctrlT[nT].intr = 1;
					pihm->prtctrlT[nT].nvar = nriver;
					pihm->prtctrlT[nT].var =
						(double **)malloc(pihm->prtctrlT[nT].nvar *
							sizeof(double *));
					pihm->prtctrlT[nT].x =
						(double **)malloc(pihm->prtctrlT[nT].nvar *
							sizeof(double *));
					pihm->prtctrlT[nT].y =
						(double **)malloc(pihm->prtctrlT[nT].nvar *
							sizeof(double *));
					pihm->prtctrlT[nT].zmin =
						(double **)malloc(pihm->prtctrlT[nT].nvar *
							sizeof(double *));
					pihm->prtctrlT[nT].zmax =
						(double **)malloc(pihm->prtctrlT[nT].nvar *
							sizeof(double *));
					for (j = 0; j < nriver; j++)
					{
						pihm->prtctrlT[nT].var[j] = &pihm->riv[j].ws.gw;
						pihm->prtctrlT[nT].x[j] = &pihm->riv[j].topo.x;
						pihm->prtctrlT[nT].y[j] = &pihm->riv[j].topo.y;
						pihm->prtctrlT[nT].zmax[j] = &pihm->riv[j].topo.zmax;
						pihm->prtctrlT[nT].zmin[j] = &pihm->riv[j].topo.zmin;
					}
					nT++;
					break;
                default:
                    break;
            }
        }
    }

    if (n > MAXPRINT)
    {
        PIHMprintf (VL_ERROR, "Error: Too many output files. ");
        PIHMprintf (VL_ERROR, "The maximum number of output files is %d.\n",
            MAXPRINT);
        PIHMexit (EXIT_FAILURE);
    }

    pihm->ctrl.nprint = n;
	pihm->ctrl.nprintT = nT;

    for (i = 0; i < pihm->ctrl.nprint; i++)
    {
        pihm->prtctrl[i].buffer =
            (double *)calloc (pihm->prtctrl[i].nvar, sizeof (double));

        pihm->prtctrl[i].counter = 0;

        if (spinup_mode)
        {
            pihm->prtctrl[i].intvl = MONTHLY_OUTPUT;
        }

    }
	for (i = 0; i < pihm->ctrl.nprintT; i++)
	{
		pihm->prtctrlT[i].buffer =
			(double *)calloc(pihm->prtctrlT[i].nvar, sizeof(double));
		pihm->prtctrlT[i].counter = 0;
		pihm->prtctrlT[i].first = 1;
	}
}
