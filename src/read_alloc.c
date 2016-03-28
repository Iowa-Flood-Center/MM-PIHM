#include "pihm.h"

void PihmFree (void **ptr)
{
    free (*ptr);
    *ptr = NULL;
}

void ReadAlloc (char *simulation, pihm_struct pihm)
{
    if (verbose_mode)
    {
        printf ("\nRead input files:\n");
    }

    /*
     * Set file names of the input files
     */
    sprintf (pihm->filename.riv,  "input/%s/%s.riv",   project, project);
    sprintf (pihm->filename.mesh, "input/%s/%s.mesh",  project, project);
    sprintf (pihm->filename.att,  "input/%s/%s.att",   project, project);
    sprintf (pihm->filename.soil, "input/%s/%s.soil",  project, project);
    sprintf (pihm->filename.geol, "input/%s/%s.geol",  project, project);
    sprintf (pihm->filename.lc,   "input/vegprmt.tbl");
    sprintf (pihm->filename.meteo, "input/%s/%s.meteo",  project, project);
    sprintf (pihm->filename.lai,  "input/%s/%s.lai",   project, project);
    sprintf (pihm->filename.bc,  "input/%s/%s.bc",   project, project);
    sprintf (pihm->filename.para, "input/%s/%s.para",  project, project);
    sprintf (pihm->filename.calib,"input/%s/%s.calib", project, simulation);
    sprintf (pihm->filename.ic, "input/%s/%s.ic",  project, simulation);
#ifdef _NOAH_
    sprintf (pihm->filename.lsm,  "input/%s/%s.lsm",   project, project);
    sprintf (pihm->filename.rad,  "input/%s/%s.rad",   project, project);
#endif
#ifdef _CYCLES_
    sprintf (pihm->filename.cycles, "input/%s/%s.cycles", project, project);
    sprintf (pihm->filename.soilinit, "input/%s/%s.soilinit", project, project);
    sprintf (pihm->filename.crop, "input/%s/%s.crop", project, project);
#endif

    /*
     * Read river input file
     */
    ReadRiv (pihm->filename.riv, &pihm->rivtbl, &pihm->shptbl, &pihm->matltbl,
        &pihm->forc);
    pihm->numriv = pihm->rivtbl.number;

    /*
     * Read mesh structure input file
     */
    ReadMesh (pihm->filename.mesh, &pihm->meshtbl);
    pihm->numele = pihm->meshtbl.numele;

    /*
     * Read attribute table input file
     */
    ReadAtt (pihm->filename.att, &pihm->atttbl, pihm->numele);

    /*
     * Read soil input file
     */
    ReadSoil (pihm->filename.soil, &pihm->soiltbl);

    /*
     * Read geology input file
     */
    //ReadGeol (pihm->filename.geol, &pihm->geoltbl);

    /*
     * Read land cover input file
     */
    ReadLC (pihm->filename.lc, &pihm->lctbl);

    /*
     * Read meteorological forcing input file
     */
    ReadForc (pihm->filename.meteo, &pihm->forc);

    /*
     * Read LAI input file
     */
    ReadLAI (pihm->filename.lai, &pihm->forc, pihm->numele, &pihm->atttbl);

    /*
     * Read source and sink input file
     */
    pihm->forc.nsource = 0;
    //ReadSS ();

    /*
     * Read boundary condition input file
     */
    pihm->forc.nbc = 0;
    ReadBC (pihm->filename.bc, &pihm->forc);

    /*
     * Read model control file
     */
    ReadPara (pihm->filename.para, &pihm->ctrl);

    /*
     * Read calibration input file
     */
    ReadCalib (pihm->filename.calib, &pihm->cal);

#ifdef _NOAH_
    /*
     * Read LSM input file
     */
    ReadLsm (pihm->filename.lsm, &pihm->latitude, &pihm->longitude,
        &pihm->ctrl, &pihm->noahtbl);

    if (pihm->ctrl.rad_mode == 1)
    {
        /*
         * Read radiation input file
         */
        ReadRad (pihm->filename.rad, &pihm->forc);
    }
#endif

#ifdef _CYCLES_
    /*
     * Read Cycles simulation control file
     */
    ReadCyclesCtrl (pihm->filename.cycles, &pihm->agtbl, pihm->numele);

    /*
     * Read soil initialization file
     */
    ReadSoilInit (pihm->filename.soilinit, &pihm->soiltbl);

    /*
     * Read crop description file
     */
    ReadCrop (pihm->filename.crop, &pihm->croptbl);

    /*
     * Read operation file
     */
    ReadOperation (&pihm->agtbl, &pihm->mgmttbl, &pihm->croptbl);
#endif
}

void ReadRiv (char *filename, rivtbl_struct *rivtbl, shptbl_struct *shptbl,
    matltbl_struct *matltbl, forc_struct *forc)
{
    int             i, j;
    FILE           *riv_file;   /* Pointer to .riv file */
    char            cmdstr[MAXSTRING];
    int             match;
    int             index;

    /*
     * Open .riv input file
     */
    riv_file = fopen (filename, "r");
    CheckFile (riv_file, filename);

    /*
     * Read river segment block
     */
    /* Read number of river segments */
    FindLine (riv_file, "BOF");
    NextLine (riv_file, cmdstr);
    ReadKeywordInt (cmdstr, "NUMRIV", &rivtbl->number);

    /* Allocate */
    rivtbl->fromnode = (int *)malloc (rivtbl->number * sizeof (int));
    rivtbl->tonode = (int *)malloc (rivtbl->number * sizeof (int));
    rivtbl->down = (int *)malloc (rivtbl->number * sizeof (int));
    rivtbl->leftele = (int *)malloc (rivtbl->number * sizeof (int));
    rivtbl->rightele = (int *)malloc (rivtbl->number * sizeof (int));
    rivtbl->shp = (int *)malloc (rivtbl->number * sizeof (int));
    rivtbl->matl = (int *)malloc (rivtbl->number * sizeof (int));
    rivtbl->bc = (int *)malloc (rivtbl->number * sizeof (int));
    rivtbl->rsvr = (int *)malloc (rivtbl->number * sizeof (int));

    /* Skip header line */
    NextLine (riv_file, cmdstr);

    /* Read river segment information */
    for (i = 0; i < rivtbl->number; i++)
    {
        NextLine (riv_file, cmdstr);
        match = sscanf (cmdstr, "%d %d %d %d %d %d %d %d %d %d",
            &index,
            &rivtbl->fromnode[i], &rivtbl->tonode[i],
            &rivtbl->down[i],
            &rivtbl->leftele[i], &rivtbl->rightele[i],
            &rivtbl->shp[i], &rivtbl->matl[i],
            &rivtbl->bc[i], &rivtbl->rsvr[i]);
        if (match != 10 || i != index - 1)
        {
            printf ("Cannot read river segment information for the %dth"
                "segment!\n", i + 1);
            printf (".riv file format error!\n");
            PihmExit (1);
        }
    }

    /*
     * Read river shape information
     */
    NextLine (riv_file, cmdstr);
    ReadKeywordInt (cmdstr, "SHAPE", &shptbl->number);

    /* Allocate */
    shptbl->depth = (double *)malloc (shptbl->number * sizeof (double));
    shptbl->intrpl_ord = (int *)malloc (shptbl->number * sizeof (int));
    shptbl->coeff = (double *)malloc (shptbl->number * sizeof (double));

    /* Skip header line */
    NextLine (riv_file, cmdstr);

    for (i = 0; i < shptbl->number; i++)
    {
        NextLine (riv_file, cmdstr);
        match = sscanf (cmdstr, "%d %lf %d %lf",
            &index, &shptbl->depth[i],
            &shptbl->intrpl_ord[i], &shptbl->coeff[i]);
        if (match != 4 || i != index - 1)
        {
            printf
                ("Cannot read river shape information for the %dth shape!\n",
                i + 1);
            printf (".riv file format error!\n");
            PihmExit (1);
        }
    }

    /*
     * Read river material information
     */
    NextLine (riv_file, cmdstr);
    ReadKeywordInt (cmdstr, "MATERIAL", &matltbl->number);

    /* Allocate */
    matltbl->rough = (double *)malloc (matltbl->number * sizeof (double));
    matltbl->cwr = (double *)malloc (matltbl->number * sizeof (double));
    matltbl->ksath = (double *)malloc (matltbl->number * sizeof (double));
    matltbl->ksatv = (double *)malloc (matltbl->number * sizeof (double));
    matltbl->bedthick = (double *)malloc (matltbl->number * sizeof (double));

    /* Skip header line */
    NextLine (riv_file, cmdstr);

    for (i = 0; i < matltbl->number; i++)
    {
        NextLine (riv_file, cmdstr);
        match = sscanf (cmdstr, "%d %lf %lf %lf %lf %lf",
            &index,
            &matltbl->rough[i], &matltbl->cwr[i],
            &matltbl->ksath[i], &matltbl->ksatv[i], &matltbl->bedthick[i]);
        if (match != 6 || i != index - 1)
        {
            printf ("Cannot read information for the %dth material!\n",
                i + 1);
            printf (".riv file format error!\n");
            PihmExit (1);
        }
    }

    /*
     * Read river boundary condition block
     */
    NextLine (riv_file, cmdstr);
    ReadKeywordInt (cmdstr, "BC", &forc->nriverbc);

    if (forc->nriverbc > 0)
    {
        forc->riverbc =
            (tsdata_struct *)malloc (forc->nriverbc * sizeof (tsdata_struct));

        for (i = 0; i < forc->nriverbc; i++)
        {
            NextLine (riv_file, cmdstr);
            match = sscanf (cmdstr, "%*s %d", &index);
            if (match != 1 || i != index - 1)
            {
                printf
                    ("Cannot read information of the %dth river boudnary condition!\n",
                    i);
                printf (".riv file format error!\n");
                PihmExit (1);
            }
            NextLine (riv_file, cmdstr);
            NextLine (riv_file, cmdstr);
            forc->riverbc[i].length =
                CountLine (riv_file, cmdstr, 2, "RIV_TS", "RES");
        }

        FindLine (riv_file, "BC");
        for (i = 0; i < forc->nriverbc; i++)
        {
            NextLine (riv_file, cmdstr);
            NextLine (riv_file, cmdstr);
            NextLine (riv_file, cmdstr);

            forc->riverbc[i].data =
                (double **)malloc ((forc->riverbc[i].length) *
                sizeof (double *));
            forc->riverbc[i].ftime =
                (int *)malloc ((forc->riverbc[i].length) * sizeof (int));
            for (j = 0; j < forc->riverbc[i].length; j++)
            {
                forc->riverbc[i].data[j] = (double *)malloc (sizeof (double));
                NextLine (riv_file, cmdstr);
                ReadTS (cmdstr, &forc->riverbc[i].ftime[j],
                    &forc->riverbc[i].data[j][0], 1);
            }
        }
    }

    /* Read Reservoir information */
    /* Empty */

    fclose (riv_file);
}

void ReadMesh (char *filename, meshtbl_struct *meshtbl)
{
    FILE           *mesh_file;  /* Pointer to .mesh file */
    int             i;
    char            cmdstr[MAXSTRING];
    int             match;
    int             index;

    /*
     * Open .mesh input file
     */
    mesh_file = fopen (filename, "r");
    CheckFile (mesh_file, filename);

    /*
     * Read element mesh block
     */
    NextLine (mesh_file, cmdstr);
    ReadKeywordInt (cmdstr, "NUMELE", &meshtbl->numele);

    meshtbl->node = (int **)malloc (meshtbl->numele * sizeof (int *));
    meshtbl->nabr = (int **)malloc (meshtbl->numele * sizeof (int *));

    /* Skip header line */
    NextLine (mesh_file, cmdstr);

    for (i = 0; i < meshtbl->numele; i++)
    {
        meshtbl->node[i] = (int *)malloc (3 * sizeof (int));
        meshtbl->nabr[i] = (int *)malloc (3 * sizeof (int));

        NextLine (mesh_file, cmdstr);
        match = sscanf (cmdstr, "%d %d %d %d %d %d %d",
            &index,
            &meshtbl->node[i][0], &meshtbl->node[i][1],
            &meshtbl->node[i][2], &meshtbl->nabr[i][0],
            &meshtbl->nabr[i][1], &meshtbl->nabr[i][2]);
        if (match != 7 || i != index - 1)
        {
            printf ("Cannot read information of the %dth element!\n", i + 1);
            printf (".mesh file format error!\n");
            PihmExit (1);
        }
    }

    /*
     * Read node block
     */
    NextLine (mesh_file, cmdstr);
    ReadKeywordInt (cmdstr, "NUMNODE", &meshtbl->numnode);

    /* Skip header line */
    NextLine (mesh_file, cmdstr);

    meshtbl->x = (double *)malloc (meshtbl->numnode * sizeof (double));
    meshtbl->y = (double *)malloc (meshtbl->numnode * sizeof (double));
    meshtbl->zmin = (double *)malloc (meshtbl->numnode * sizeof (double));
    meshtbl->zmax = (double *)malloc (meshtbl->numnode * sizeof (double));

    for (i = 0; i < meshtbl->numnode; i++)
    {
        NextLine (mesh_file, cmdstr);
        match = sscanf (cmdstr, "%d %lf %lf %lf %lf",
            &index,
            &meshtbl->x[i], &meshtbl->y[i],
            &meshtbl->zmin[i], &meshtbl->zmax[i]);
        if (match != 5 || i != index - 1)
        {
            printf ("Cannot read information of the %dth node!\n", i + 1);
            printf (".mesh file format error!\n");
            PihmExit (1);
        }
    }

    /* finish reading mesh_files */
    fclose (mesh_file);
}

void ReadAtt (char *filename, atttbl_struct *atttbl, int numele)
{
    int             i;
    FILE           *att_file;   /* Pointer to .att file */
    char            cmdstr[MAXSTRING];
    int             match;
    int             index;

    att_file = fopen (filename, "r");
    CheckFile (att_file, filename);

    atttbl->soil = (int *)malloc (numele * sizeof (int));
    atttbl->geol = (int *)malloc (numele * sizeof (int));
    atttbl->lc = (int *)malloc (numele * sizeof (int));
    atttbl->bc = (int **)malloc (numele * sizeof (int *));
    for (i = 0; i < numele; i++)
    {
        atttbl->bc[i] = (int *)malloc (3 * sizeof (int));
    }
    atttbl->meteo = (int *)malloc (numele * sizeof (int));
    atttbl->lai = (int *)malloc (numele * sizeof (int));
    atttbl->source = (int *)malloc (numele * sizeof (int));
    atttbl->macropore = (int *)malloc (numele * sizeof (int));

    NextLine (att_file, cmdstr);
    for (i = 0; i < numele; i++)
    {
        NextLine (att_file, cmdstr);
        match = sscanf (cmdstr, "%d %d %d %d %d %d %d %d %d %d %d", &index,
            &atttbl->soil[i], &atttbl->geol[i], &atttbl->lc[i],
            &atttbl->meteo[i], &atttbl->lai[i], &atttbl->source[i],
            &atttbl->bc[i][0], &atttbl->bc[i][1], &atttbl->bc[i][2],
            &atttbl->macropore[i]);
        if (match != 11)
        {
            printf ("Cannot read information of the %dth element!\n", i + 1);
            printf (".att file format error!\n");
            PihmExit (1);
        }
    }

    /* finish reading att_files */
    fclose (att_file);
}

void ReadSoil (char *filename, soiltbl_struct *soiltbl)
{
    FILE           *soil_file;  /* Pointer to .soil file */
    int             i;
    char            cmdstr[MAXSTRING];
    int             match;
    int             index;
    int             texture;
    const int       TOPSOIL = 1;
    const int       SUBSOIL = 0;
    int             ptf_used = 0;

    soil_file = fopen (filename, "r");
    CheckFile (soil_file, filename);

    /* Start reading soil file */
    NextLine (soil_file, cmdstr);
    ReadKeywordInt (cmdstr, "NUMSOIL", &soiltbl->number);

    soiltbl->silt = (double *)malloc (soiltbl->number * sizeof (double));
    soiltbl->clay = (double *)malloc (soiltbl->number * sizeof (double));
    soiltbl->om = (double *)malloc (soiltbl->number * sizeof (double));
    soiltbl->bd = (double *)malloc (soiltbl->number * sizeof (double));
    soiltbl->kinfv = (double *)malloc (soiltbl->number * sizeof (double));
    soiltbl->ksatv = (double *)malloc (soiltbl->number * sizeof (double));
    soiltbl->ksath = (double *)malloc (soiltbl->number * sizeof (double));
    soiltbl->smcmax = (double *)malloc (soiltbl->number * sizeof (double));
    soiltbl->smcmin = (double *)malloc (soiltbl->number * sizeof (double));
    soiltbl->qtz = (double *)malloc (soiltbl->number * sizeof (double));
    soiltbl->alpha = (double *)malloc (soiltbl->number * sizeof (double));
    soiltbl->beta = (double *)malloc (soiltbl->number * sizeof (double));
    soiltbl->areafh = (double *)malloc (soiltbl->number * sizeof (double));
    soiltbl->areafv = (double *)malloc (soiltbl->number * sizeof (double));
    soiltbl->dmac = (double *)malloc (soiltbl->number * sizeof (double));
    soiltbl->smcref = (double *)malloc (soiltbl->number * sizeof (double));
    soiltbl->smcwlt = (double *)malloc (soiltbl->number * sizeof (double));

    /* Skip header line */
    NextLine (soil_file, cmdstr);

    for (i = 0; i < soiltbl->number; i++)
    {
        NextLine (soil_file, cmdstr);
        match = sscanf (cmdstr,
            "%d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
            &index, &soiltbl->silt[i], &soiltbl->clay[i], &soiltbl->om[i],
            &soiltbl->bd[i],
            &soiltbl->kinfv[i], &soiltbl->ksatv[i], &soiltbl->ksath[i],
            &soiltbl->smcmax[i], &soiltbl->smcmin[i],
            &soiltbl->alpha[i], &soiltbl->beta[i],
            &soiltbl->areafh[i], &soiltbl->areafv[i],
            &soiltbl->dmac[i], &soiltbl->qtz[i]);

        if (match != 16 || i != index - 1)
        {
            printf ("Cannot read information of the %dth soil type!\n",
                i + 1);
            printf (".soil file format error!\n");
            PihmExit (1);
        }

        /* Fill in missing organic matter and bulk density values */
        soiltbl->om[i] = (soiltbl->om[i] > 0.0) ? soiltbl->om[i] : 2.5;
        soiltbl->bd[i] = (soiltbl->bd[i] > 0.0) ? soiltbl->bd[i] : 1.3;

        /* Fill missing hydraulic properties using PTFs */
        if (soiltbl->kinfv[i] < 0.0)
        {
            soiltbl->kinfv[i] =
                PtfKV (soiltbl->silt[i], soiltbl->clay[i], soiltbl->om[i],
                soiltbl->bd[i], TOPSOIL);
            ptf_used = 1;
        }
        if (soiltbl->ksatv[i] < 0.0)
        {
            soiltbl->ksatv[i] =
                PtfKV (soiltbl->silt[i], soiltbl->clay[i], soiltbl->om[i],
                soiltbl->bd[i], SUBSOIL);
            ptf_used = 1;
        }
        if (soiltbl->ksath[i] < 0.0)
        {
            soiltbl->ksath[i] = 10.0 * soiltbl->ksatv[i];
            ptf_used = 1;
        }
        if (soiltbl->smcmax[i] < 0.0)
        {
            soiltbl->smcmax[i] =
                PtfThetaS (soiltbl->silt[i], soiltbl->clay[i], soiltbl->om[i],
                soiltbl->bd[i], SUBSOIL);
            ptf_used = 1;
        }
        if (soiltbl->smcmin[i] < 0.0)
        {
            soiltbl->smcmin[i] =
                PtfThetaR (soiltbl->silt[i], soiltbl->clay[i], soiltbl->om[i],
                soiltbl->bd[i], SUBSOIL);
            ptf_used = 1;
        }
        if (soiltbl->alpha[i] < 0.0)
        {
            soiltbl->alpha[i] =
                PtfAlpha (soiltbl->silt[i], soiltbl->clay[i], soiltbl->om[i],
                soiltbl->bd[i], SUBSOIL);
            ptf_used = 1;
        }
        if (soiltbl->beta[i] < 0.0)
        {
            soiltbl->beta[i] =
                PtfBeta (soiltbl->silt[i], soiltbl->clay[i], soiltbl->om[i],
                soiltbl->bd[i], SUBSOIL);
            ptf_used = 1;
        }
        if (soiltbl->qtz[i] < 0.0)
        {
            texture = SoilTex (soiltbl->silt[i], soiltbl->clay[i]);
            soiltbl->qtz[i] = Qtz (texture);
            ptf_used = 1;
        }

        /* Calculate field capacity and wilting point */
        soiltbl->smcref[i] =
            FieldCapacity (soiltbl->alpha[i], soiltbl->beta[i],
            soiltbl->ksatv[i], soiltbl->smcmax[i], soiltbl->smcmin[i]);
        soiltbl->smcwlt[i] =
            WiltingPoint (soiltbl->smcmax[i], soiltbl->smcmin[i],
            soiltbl->alpha[i], soiltbl->beta[i]);
    }

    NextLine (soil_file, cmdstr);
    ReadKeywordDouble (cmdstr, "DINF", &soiltbl->dinf);

    NextLine (soil_file, cmdstr);
    ReadKeywordDouble (cmdstr, "KMACV_RO", &soiltbl->kmacv_ro);

    NextLine (soil_file, cmdstr);
    ReadKeywordDouble (cmdstr, "KMACH_RO", &soiltbl->kmach_ro);

    if (ptf_used)
    {
        printf ("%-7s\t%-15s\t%-15s\t%-15s\t%-7s\t%-7s\t%-7s\t%-7s\t%-7s\n",
            "TYPE", "KINFV", "KSATV", "KSATH", "SMCMAX", "SMCMIN", "ALPHA",
            "BETA", "QTZ");
        for (i = 0; i < soiltbl->number; i++)
        {
            printf
                ("%-7d\t%-15.3le\t%-15.3le\t%-15.3le\t%-7.3lf\t%-7.3lf\t"
                "%-7.3lf\t%-7.3lf\t%-7.3lf\n",
                i + 1, soiltbl->kinfv[i], soiltbl->ksatv[i],
                soiltbl->ksath[i], soiltbl->smcmax[i], soiltbl->smcmin[i],
                soiltbl->alpha[i], soiltbl->beta[i], soiltbl->qtz[i]);
        }
    }

    fclose (soil_file);
}

void ReadGeol (char *filename, geoltbl_struct *geoltbl)
{
    FILE           *geol_file;  /* Pointer to .geol file */
    int             i;
    char            cmdstr[MAXSTRING];
    int             match;
    int             index;

    geol_file = fopen (filename, "r");
    CheckFile (geol_file, filename);

    /* start reading geol_file */
    NextLine (geol_file, cmdstr);
    match = sscanf (cmdstr, "%d", &geoltbl->number);
    if (match != 1)
    {
        printf ("Cannot read number of geology types!\n");
        printf (".geol file format error!\n");
        PihmExit (1);
    }

    geoltbl->silt = (double *)malloc (geoltbl->number * sizeof (double));
    geoltbl->clay = (double *)malloc (geoltbl->number * sizeof (double));
    geoltbl->om = (double *)malloc (geoltbl->number * sizeof (double));
    geoltbl->bd = (double *)malloc (geoltbl->number * sizeof (double));
    geoltbl->ksath = (double *)malloc (geoltbl->number * sizeof (double));
    geoltbl->ksatv = (double *)malloc (geoltbl->number * sizeof (double));
    geoltbl->smcmax = (double *)malloc (geoltbl->number * sizeof (double));
    geoltbl->smcmin = (double *)malloc (geoltbl->number * sizeof (double));
    geoltbl->alpha = (double *)malloc (geoltbl->number * sizeof (double));
    geoltbl->beta = (double *)malloc (geoltbl->number * sizeof (double));

    for (i = 0; i < geoltbl->number; i++)
    {
        NextLine (geol_file, cmdstr);
        match = sscanf (cmdstr, "%d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
            &index,
            &geoltbl->silt[i], &geoltbl->clay[i], &geoltbl->om[i],
            &geoltbl->bd[i],
            &geoltbl->ksatv[i], &geoltbl->ksath[i],
            &geoltbl->smcmax[i], &geoltbl->smcmin[i],
            &geoltbl->alpha[i], &geoltbl->beta[i]);
        if (match != 11 || i != index - 1)
        {
            printf ("Cannot read information of the %dth geology type!\n",
                i + 1);
            printf (".geol file format error!\n");
            PihmExit (1);
        }
    }

    fclose (geol_file);
}

void ReadLC (char *filename, lctbl_struct *lctbl)
{
    FILE           *lc_file;    /* Pointer to .lc file */
    int             i;
    char            cmdstr[MAXSTRING];
    int             match;
    int             index;

    lc_file = fopen (filename, "r");
    CheckFile (lc_file, filename);

    /* Start reading land cover file */
    NextLine (lc_file, cmdstr);
    ReadKeywordInt (cmdstr, "NUMLC", &lctbl->number);

    lctbl->laimax = (double *)malloc (lctbl->number * sizeof (double));
    lctbl->laimin = (double *)malloc (lctbl->number * sizeof (double));
    lctbl->vegfrac = (double *)malloc (lctbl->number * sizeof (double));
    lctbl->albedomin = (double *)malloc (lctbl->number * sizeof (double));
    lctbl->albedomax = (double *)malloc (lctbl->number * sizeof (double));
    lctbl->emissmin = (double *)malloc (lctbl->number * sizeof (double));
    lctbl->emissmax = (double *)malloc (lctbl->number * sizeof (double));
    lctbl->z0min = (double *)malloc (lctbl->number * sizeof (double));
    lctbl->z0max = (double *)malloc (lctbl->number * sizeof (double));
    lctbl->hs = (double *)malloc (lctbl->number * sizeof (double));
    lctbl->snup = (double *)malloc (lctbl->number * sizeof (double));
    lctbl->rgl = (double *)malloc (lctbl->number * sizeof (double));
    lctbl->rsmin = (double *)malloc (lctbl->number * sizeof (double));
    lctbl->rough = (double *)malloc (lctbl->number * sizeof (double));
    lctbl->rzd = (double *)malloc (lctbl->number * sizeof (double));

    /* Skip header line */
    NextLine (lc_file, cmdstr);

    for (i = 0; i < lctbl->number; i++)
    {
        NextLine (lc_file, cmdstr);
        match =
            sscanf (cmdstr,
            "%d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
            &index, &lctbl->vegfrac[i], &lctbl->rzd[i], &lctbl->rsmin[i],
            &lctbl->rgl[i], &lctbl->hs[i], &lctbl->snup[i],
            &lctbl->laimin[i], &lctbl->laimax[i], &lctbl->emissmin[i],
            &lctbl->emissmax[i], &lctbl->albedomin[i],
            &lctbl->albedomax[i], &lctbl->z0min[i], &lctbl->z0max[i],
            &lctbl->rough[i]);
        if (match != 16 || i != index - 1)
        {
            printf ("Cannot read information of the %dth landcover type!\n",
                i + 1);
            printf ("Landcover file format error!\n");
            PihmExit (1);
        }
    }

    NextLine (lc_file, cmdstr);
    ReadKeywordDouble (cmdstr, "TOPT_DATA", &lctbl->topt);

    NextLine (lc_file, cmdstr);
    ReadKeywordDouble (cmdstr, "CFACTR_DATA", &lctbl->cfactr);

    NextLine (lc_file, cmdstr);
    ReadKeywordDouble (cmdstr, "RSMAX_DATA", &lctbl->rsmax);

    NextLine (lc_file, cmdstr);
    ReadKeywordInt (cmdstr, "BARE", &lctbl->bare);

    NextLine (lc_file, cmdstr);
    ReadKeywordInt (cmdstr, "NATURAL", &lctbl->natural);

    fclose (lc_file);
}

void ReadForc (char *filename, forc_struct *forc)
{
    FILE           *meteo_file;  /* Pointer to .forc file */
    char            cmdstr[MAXSTRING];
    int             i, j;
    int             match;
    int             index;

    meteo_file = fopen (filename, "r");
    CheckFile (meteo_file, filename);

    FindLine (meteo_file, "BOF");

    forc->nmeteo = CountOccurance (meteo_file, "METEO_TS");

    FindLine (meteo_file, "BOF");
    if (forc->nmeteo > 0)
    {
        forc->meteo =
            (tsdata_struct *)malloc (forc->nmeteo * sizeof (tsdata_struct));

        NextLine (meteo_file, cmdstr);
        for (i = 0; i < forc->nmeteo; i++)
        {
            match = sscanf (cmdstr, "%*s %d %*s %lf",
                &index, &forc->meteo[i].zlvl_wind);
            if (match != 2 || i != index - 1)
            {
                printf
                    ("Cannot read information of the %dth forcing series!\n",
                    i);
                printf (".forc file format error!\n");
                PihmExit (1);
            }
            /* Skip header lines */
            NextLine (meteo_file, cmdstr);
            NextLine (meteo_file, cmdstr);
            forc->meteo[i].length =
                CountLine (meteo_file, cmdstr, 1, "METEO_TS");
        }

        /* Rewind and read */
        FindLine (meteo_file, "BOF");
        for (i = 0; i < forc->nmeteo; i++)
        {
            /* Skip header lines */
            NextLine (meteo_file, cmdstr);
            NextLine (meteo_file, cmdstr);
            NextLine (meteo_file, cmdstr);

            forc->meteo[i].ftime =
                (int *)malloc (forc->meteo[i].length * sizeof (int));
            forc->meteo[i].data =
                (double **)malloc (forc->meteo[i].length * sizeof (double *));
            for (j = 0; j < forc->meteo[i].length; j++)
            {
                forc->meteo[i].data[j] =
                    (double *)malloc (NUM_METEO_VAR * sizeof (double));
                NextLine (meteo_file, cmdstr);
                ReadTS (cmdstr, &forc->meteo[i].ftime[j],
                    &forc->meteo[i].data[j][0], NUM_METEO_VAR);
            }
        }
    }

    fclose (meteo_file);
}

void ReadLAI (char *filename, forc_struct *forc, int numele,
    const atttbl_struct *atttbl)
{
    char            cmdstr[MAXSTRING];
    int             read_lai = 0;
    FILE           *lai_file;
    int             i, j;
    int             index;

    for (i = 0; i < numele; i++)
    {
        if (atttbl->lai[i] > 0)
        {
            read_lai = 1;
            break;
        }
    }

    forc->nlai = 0;

    if (read_lai)
    {
        lai_file = fopen (filename, "r");
        CheckFile (lai_file, filename);

        /* start reading lai_file */
        FindLine (lai_file, "BOF");

        forc->nlai = CountOccurance (lai_file, "LAI_TS");

        FindLine (lai_file, "BOF");
        if (forc->nlai > 0)
        {
            forc->lai =
                (tsdata_struct *)malloc (forc->nlai * sizeof (tsdata_struct));

            NextLine (lai_file, cmdstr);
            for (i = 0; i < forc->nlai; i++)
            {
                ReadKeywordInt (cmdstr, "LAI_TS", &index);
                if (i != index - 1)
                {
                    printf
                        ("Cannot read information of the %dth LAI series!\n",
                        i);
                    printf (".lai file format error!\n");
                    PihmExit (1);
                }
                /* Skip header lines */
                NextLine (lai_file, cmdstr);
                NextLine (lai_file, cmdstr);
                forc->lai[i].length =
                    CountLine (lai_file, cmdstr, 1, "LAI_TS");
            }

            /* Rewind and read */
            FindLine (lai_file, "BOF");
            for (i = 0; i < forc->nlai; i++)
            {
                /* Skip header lines */
                NextLine (lai_file, cmdstr);
                NextLine (lai_file, cmdstr);
                NextLine (lai_file, cmdstr);

                forc->lai[i].ftime =
                    (int *)malloc (forc->lai[i].length * sizeof (int));
                forc->lai[i].data =
                    (double **)malloc (forc->lai[i].length *
                    sizeof (double *));
                for (j = 0; j < forc->lai[i].length; j++)
                {
                    forc->lai[i].data[j] = (double *)malloc (sizeof (double));
                    NextLine (lai_file, cmdstr);
                    ReadTS (cmdstr, &forc->lai[i].ftime[j],
                        &forc->lai[i].data[j][0], 1);
                }
            }
        }

        fclose (lai_file);
    }
}


void ReadBC (char *filename, forc_struct *forc)
{
    int             i, j;
    FILE           *bc_file;   /* Pointer to .ibc file */
    char            cmdstr[MAXSTRING];
    int             match;
    int             index;

    bc_file = fopen (filename, "r");
    CheckFile (bc_file, filename);

    /*
     * Start reading bc_file 
     */
    FindLine (bc_file, "BOF");
    NextLine (bc_file, cmdstr);
    match = sscanf (cmdstr, "%*s %d", &forc->nbc);
    if (match != 1)
    {
        printf ("Cannot read number of boundary condition time series!\n");
        printf (".ibc file format error!\n");
        PihmExit (1);
    }

    if (forc->nbc > 0)
    {
        forc->bc =
            (tsdata_struct *)malloc (forc->nbc * sizeof (tsdata_struct));

        for (i = 0; i < forc->nbc; i++)
        {
            NextLine (bc_file, cmdstr);
            match = sscanf (cmdstr, "%*s %d", &index);
            if (match != 1 || i != index - 1)
            {
                printf
                    ("Cannot read information of the %dth boundary condition series!\n",
                    i);
                printf (".ibc file format error!\n");
                PihmExit (1);
            }
            /* Skip header lines */
            NextLine (bc_file, cmdstr);
            NextLine (bc_file, cmdstr);

            forc->bc[i].length = CountLine (bc_file, cmdstr, 1, "BC_TS");
        }

        /* Rewind and read */
        FindLine (bc_file, "NUM_BC_TS");
        for (i = 0; i < forc->nbc; i++)
        {
            /* Skip header lines */
            NextLine (bc_file, cmdstr);
            NextLine (bc_file, cmdstr);
            NextLine (bc_file, cmdstr);

            forc->bc[i].ftime =
                (int *)malloc (forc->bc[i].length * sizeof (int));
            forc->bc[i].data =
                (double **)malloc (forc->bc[i].length * sizeof (double *));
            for (j = 0; j < forc->bc[i].length; j++)
            {
                forc->bc[i].data[j] = (double *)malloc (sizeof (double));
                NextLine (bc_file, cmdstr);
                ReadTS (cmdstr, &forc->bc[i].ftime[j],
                    &forc->bc[i].data[j][0], 1);
            }
        }
    }

    fclose (bc_file);
}

void ReadPara (char *filename, ctrl_struct *ctrl)
{
    FILE           *para_file;  /* Pointer to .para file */
    char            cmdstr[MAXSTRING];
    int             i;

    for (i = 0; i < NUM_PRINT; i++)
    {
        ctrl->prtvrbl[i] = 0;
    }

    para_file = fopen (filename, "r");
    CheckFile (para_file, filename);

    /* start reading para_file */
    /* Read through parameter file to find parameters */
    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "INIT_MODE", &ctrl->init_type);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "ASCII_OUTPUT", &ctrl->ascii);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "WRITE_IC", &ctrl->write_ic);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "UNSAT_MODE", &ctrl->unsat_mode);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "SURF_MODE", &ctrl->surf_mode);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "RIV_MODE", &ctrl->riv_mode);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "SOLVER", &ctrl->solver);

    NextLine (para_file, cmdstr);
    ReadKeywordDouble (cmdstr, "ABSTOL", &ctrl->abstol);

    NextLine (para_file, cmdstr);
    ReadKeywordDouble (cmdstr, "RELTOL", &ctrl->reltol);

    NextLine (para_file, cmdstr);
    ReadKeywordDouble (cmdstr, "INIT_SOLVER_STEP", &ctrl->initstep);

    NextLine (para_file, cmdstr);
    ReadKeywordDouble (cmdstr, "MAX_SOLVER_STEP", &ctrl->maxstep);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "LSM_STEP", &ctrl->etstep);

    NextLine (para_file, cmdstr);
    ReadKeywordTime (cmdstr, "START", &ctrl->starttime);

    NextLine (para_file, cmdstr);
    ReadKeywordTime (cmdstr, "END", &ctrl->endtime);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "MODEL_STEPSIZE", &ctrl->stepsize);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "SURF", &ctrl->prtvrbl[SURF_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "UNSAT", &ctrl->prtvrbl[UNSAT_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "GW", &ctrl->prtvrbl[GW_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "RIVSTG", &ctrl->prtvrbl[RIVSTG_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "RIVGW", &ctrl->prtvrbl[RIVGW_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "SNOW", &ctrl->prtvrbl[SNOW_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "CMC", &ctrl->prtvrbl[CMC_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "INFIL", &ctrl->prtvrbl[INFIL_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "RECHARGE", &ctrl->prtvrbl[RECHARGE_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "EC", &ctrl->prtvrbl[EC_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "ETT", &ctrl->prtvrbl[ETT_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "EDIR", &ctrl->prtvrbl[EDIR_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "RIVFLX0", &ctrl->prtvrbl[RIVFLX0_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "RIVFLX1", &ctrl->prtvrbl[RIVFLX1_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "RIVFLX2", &ctrl->prtvrbl[RIVFLX2_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "RIVFLX3", &ctrl->prtvrbl[RIVFLX3_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "RIVFLX4", &ctrl->prtvrbl[RIVFLX4_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "RIVFLX5", &ctrl->prtvrbl[RIVFLX5_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "RIVFLX6", &ctrl->prtvrbl[RIVFLX6_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "RIVFLX7", &ctrl->prtvrbl[RIVFLX7_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "RIVFLX8", &ctrl->prtvrbl[RIVFLX8_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "RIVFLX9", &ctrl->prtvrbl[RIVFLX9_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "RIVFLX10", &ctrl->prtvrbl[RIVFLX10_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "SUBFLX", &ctrl->prtvrbl[SUBFLX_CTRL]);

    NextLine (para_file, cmdstr);
    ReadKeywordInt (cmdstr, "SURFFLX", &ctrl->prtvrbl[SURFFLX_CTRL]);

    fclose (para_file);

    if (ctrl->etstep < ctrl->stepsize || ctrl->etstep % ctrl->stepsize > 0)
    {
        printf
            ("ERROR: LSM (ET) step size should be an integral multiple of"
            "model step size!\n");
        PihmExit (1);
    }
}

void ReadCalib (char *filename, calib_struct *cal)
{
    char            cmdstr[MAXSTRING];
    FILE           *global_calib;       /* Pointer to .calib file */

    global_calib = fopen (filename, "r");
    CheckFile (global_calib, filename);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "KSATH", &cal->ksath);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "KSATV", &cal->ksatv);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "KINF", &cal->kinfv);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "KMACSATH", &cal->kmach);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "KMACSATV", &cal->kmacv);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "DINF", &cal->dinf);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "DROOT", &cal->rzd);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "DMAC", &cal->dmac);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "POROSITY", &cal->porosity);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "ALPHA", &cal->alpha);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "BETA", &cal->beta);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "MACVF", &cal->areafv);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "MACHF", &cal->areafh);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "VEGFRAC", &cal->vegfrac);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "ALBEDO", &cal->albedo);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "ROUGH", &cal->rough);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "PRCP", &cal->prcp);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "SFCTMP", &cal->sfctmp);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "EC", &cal->ec);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "ETT", &cal->ett);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "EDIR", &cal->edir);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "ROUGH_RIV", &cal->rivrough);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "KRIVH", &cal->rivksath);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "KRIVV", &cal->rivksatv);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "BEDTHCK", &cal->rivbedthick);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "RIV_DPTH", &cal->rivdepth);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "RIV_WDTH", &cal->rivshpcoeff);

#ifdef _RT_
    CS->Cal.PCO2 = 1.0;
    CS->Cal.Keq = 1.0;
    CS->Cal.Site_den = 1.0;
    CS->Cal.SSA = 1.0;
    CS->Cal.Prep_conc = 1.0;
#endif

#ifdef _NOAH_
    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "DRIP", &cal->drip);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "CMCMAX", &cal->intcp);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "RS", &cal->rsmin);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "CZIL", &cal->czil);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "FXEXP", &cal->fxexp);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "CFACTR", &cal->cfactr);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "RGL", &cal->rgl);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "HS", &cal->hs);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "REFSMC", &cal->thetaref);

    NextLine (global_calib, cmdstr);
    ReadKeywordDouble (cmdstr, "WLTSMC", &cal->thetaw);
#endif

    /* finish reading calib file */
    fclose (global_calib);
}

void ReadIC (char *filename, elem_struct *elem, int numele,
    river_struct *riv, int numriv)
{
    FILE           *ic_file;
    int             i;
    int             sz;
    int             elemsz;
    int             rivsz;
#ifdef _NOAH_
    int             j;
#endif

    ic_file = fopen (filename, "rb");
    CheckFile (ic_file, filename);

    elemsz = 5 * sizeof (double);
    rivsz = 2 * sizeof (double);
#ifdef _NOAH_
    elemsz += (2 + 3 * MAXLYR) * sizeof (double);
#endif

    fseek (ic_file, 0L, SEEK_END);
    sz = ftell (ic_file);

    if (sz != elemsz * numele + rivsz * numriv)
    {
        printf ("\nERROR:.ic file size does not match!\n");
        PihmExit (1);
    }

    fseek (ic_file, 0L, SEEK_SET);

    for (i = 0; i < numele; i++)
    {
        fread (&elem[i].ic.intcp, sizeof (double), 1, ic_file);
        fread (&elem[i].ic.sneqv, sizeof (double), 1, ic_file);
        fread (&elem[i].ic.surf, sizeof (double), 1, ic_file);
        fread (&elem[i].ic.unsat, sizeof (double), 1, ic_file);
        fread (&elem[i].ic.gw, sizeof (double), 1, ic_file);
#ifdef _NOAH_
        fread (&elem[i].ic.t1, sizeof (double), 1, ic_file);
        fread (&elem[i].ic.snowh, sizeof (double), 1, ic_file);

        for (j = 0; j < MAXLYR; j++)
        {
            fread (&elem[i].ic.stc[j], sizeof (double), 1, ic_file);
        }
        for (j = 0; j < MAXLYR; j++)
        {
            fread (&elem[i].ic.smc[j], sizeof (double), 1, ic_file);
        }
        for (j = 0; j < MAXLYR; j++)
        {
            fread (&elem[i].ic.sh2o[j], sizeof (double), 1, ic_file);
        }
#endif
    }
    for (i = 0; i < numriv; i++)
    {
        fread (&riv[i].ic.stage, sizeof (double), 1, ic_file);
        fread (&riv[i].ic.gw, sizeof (double), 1, ic_file);
    }

    fclose (ic_file);
}

void FreeData (pihm_struct pihm)
{
    int             i, j;

    /* Free river input structure */
    free (pihm->rivtbl.fromnode);
    free (pihm->rivtbl.tonode);
    free (pihm->rivtbl.down);
    free (pihm->rivtbl.leftele);
    free (pihm->rivtbl.rightele);
    free (pihm->rivtbl.shp);
    free (pihm->rivtbl.matl);
    free (pihm->rivtbl.bc);
    free (pihm->rivtbl.rsvr);

    free (pihm->shptbl.depth);
    free (pihm->shptbl.intrpl_ord);
    free (pihm->shptbl.coeff);

    free (pihm->matltbl.rough);
    free (pihm->matltbl.cwr);
    free (pihm->matltbl.ksath);
    free (pihm->matltbl.ksatv);
    free (pihm->matltbl.bedthick);

    /* Free mesh input structure */
    for (i = 0; i < pihm->meshtbl.numele; i++)
    {
        free (pihm->meshtbl.node[i]);
        free (pihm->meshtbl.nabr[i]);
    }
    free (pihm->meshtbl.node);
    free (pihm->meshtbl.nabr);
    free (pihm->meshtbl.x);
    free (pihm->meshtbl.y);
    free (pihm->meshtbl.zmin);
    free (pihm->meshtbl.zmax);

    /* Free attribute input structure */
    for (i = 0; i < pihm->meshtbl.numele; i++)
    {
        free (pihm->atttbl.bc[i]);
    }
    free (pihm->atttbl.soil);
    free (pihm->atttbl.geol);
    free (pihm->atttbl.lc);
    free (pihm->atttbl.bc);
    free (pihm->atttbl.meteo);
    free (pihm->atttbl.lai);
    free (pihm->atttbl.source);
    free (pihm->atttbl.macropore);

    /* Free soil input structure */
    free (pihm->soiltbl.silt);
    free (pihm->soiltbl.clay);
    free (pihm->soiltbl.om);
    free (pihm->soiltbl.bd);
    free (pihm->soiltbl.kinfv);
    free (pihm->soiltbl.ksatv);
    free (pihm->soiltbl.ksath);
    free (pihm->soiltbl.smcmax);
    free (pihm->soiltbl.smcmin);
    free (pihm->soiltbl.qtz);
    free (pihm->soiltbl.alpha);
    free (pihm->soiltbl.beta);
    free (pihm->soiltbl.areafh);
    free (pihm->soiltbl.areafv);
    free (pihm->soiltbl.dmac);
    free (pihm->soiltbl.smcref);
    free (pihm->soiltbl.smcwlt);

    /* Free geol input structure */
    //free (pihm->geol_tbl.ksath);
    //free (pihm->geol_tbl.ksatv);
    //free (pihm->geol_tbl.smcmax);
    //free (pihm->geol_tbl.smcmin);
    //free (pihm->geol_tbl.alpha);
    //free (pihm->geol_tbl.beta);
    //free (pihm->geol_tbl.areafv);
    //free (pihm->geol_tbl.kmach);
    //free (pihm->geol_tbl.dmac);

    /* Free landcover input structure */
    free (pihm->lctbl.laimax);
    free (pihm->lctbl.laimin);
    free (pihm->lctbl.vegfrac);
    free (pihm->lctbl.albedomin);
    free (pihm->lctbl.albedomax);
    free (pihm->lctbl.emissmin);
    free (pihm->lctbl.emissmax);
    free (pihm->lctbl.z0min);
    free (pihm->lctbl.z0max);
    free (pihm->lctbl.hs);
    free (pihm->lctbl.snup);
    free (pihm->lctbl.rgl);
    free (pihm->lctbl.rsmin);
    free (pihm->lctbl.rough);
    free (pihm->lctbl.rzd);

    /* Free forcing input structure */
    if (pihm->forc.nriverbc > 0)
    {
        for (i = 0; i < pihm->forc.nriverbc; i++)
        {
            for (j = 0; j < pihm->forc.riverbc[i].length; j++)
            {
                free (pihm->forc.riverbc[i].data[j]);
            }
            free (pihm->forc.riverbc[i].ftime);
            free (pihm->forc.riverbc[i].data);
        }
        free (pihm->forc.riverbc);
    }

    if (pihm->forc.nmeteo > 0)
    {
        for (i = 0; i < pihm->forc.nmeteo; i++)
        {
            for (j = 0; j < pihm->forc.meteo[i].length; j++)
            {
                free (pihm->forc.meteo[i].data[j]);
            }
            free (pihm->forc.meteo[i].ftime);
            free (pihm->forc.meteo[i].data);
            free (pihm->forc.meteo[i].value);
        }
        free (pihm->forc.meteo);
    }

    if (pihm->forc.nlai > 0)
    {
        for (i = 0; i < pihm->forc.nlai; i++)
        {
            for (j = 0; j < pihm->forc.lai[i].length; j++)
            {
                free (pihm->forc.lai[i].data[j]);
            }
            free (pihm->forc.lai[i].ftime);
            free (pihm->forc.lai[i].data);
            free (pihm->forc.lai[i].value);
        }
        free (pihm->forc.lai);
    }

#ifdef _NOAH_
    if (pihm->forc.nrad > 0)
    {
        for (i = 0; i < pihm->forc.nrad; i++)
        {
            for (j = 0; j < pihm->forc.rad[i].length; j++)
            {
                free (pihm->forc.rad[i].data[j]);
            }
            free (pihm->forc.rad[i].ftime);
            free (pihm->forc.rad[i].data);
            free (pihm->forc.rad[i].value);
        }
        free (pihm->forc.rad);
    }
#endif

    free (pihm->ctrl.tout);

    for (i = 0; i < pihm->ctrl.nprint; i++)
    {
        free (pihm->prtctrl[i].vrbl);
        free (pihm->prtctrl[i].buffer);
    }

    free (pihm->elem);
    free (pihm->riv);
}
