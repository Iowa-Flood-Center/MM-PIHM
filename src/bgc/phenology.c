#include "pihm.h"

void Phenology (const epconst_struct *epc, epvar_struct *epv,
    const estate_struct *es, cstate_struct *cs, cflux_struct *cf,
    nstate_struct *ns, nflux_struct *nf, double dt)
{
    /* Define the phenology signals for cases in which the phenology signals
     * are constant between years */
    if (epc->evergreen)
    {
        EvergreenPhenology (epc, epv, cs);
    }
    else
    {
        if (epc->woody)
        {
            SeasonDecidPhenology (epc, epv, es, cs, dt);
        }
    }

    OnsetGrowth (epc, epv, cs, cf, ns, nf, dt);

    OffsetLitterfall (epc, epv, cs, cf, ns, nf, dt);

    BackgroundLitterfall (epc, epv, cs, cf, ns, nf, dt);

    LivewoodTurnover (epc, epv, cs, cf, ns, nf, dt);
}

void EvergreenPhenology (const epconst_struct *epc, epvar_struct *epv,
    cstate_struct *cs)
{
    epv->dormant_flag = 0;
    epv->onset_flag = 0;
    epv->offset_flag = 0;
    epv->bg_leafc_litfall_rate =
        cs->leafc * epc->leaf_turnover / 365.0 / DAYINSEC;
    epv->bg_frootc_litfall_rate =
        cs->frootc * epc->froot_turnover / 365.0 / DAYINSEC;
}

void SeasonDecidPhenology (const epconst_struct *epc, epvar_struct *epv,
    const estate_struct *es, const cstate_struct *cs, double dt)
{
    int             ws_flag;
    double          onset_critsum;
    double          critdayl = 39300.0; /* seconds */
    double          tsoil;

    onset_critsum = exp (4.795 + 0.129 * (epv->annavg_t2m - TFREEZ));

    tsoil = es->stc[0] - TFREEZ;

    epv->bg_leafc_litfall_rate = 0.0;
    epv->bg_frootc_litfall_rate = 0.0;

    /* Set flag for solstice period
     * (winter->summer = 1, summer->winter = 0) */
    if (epv->dayl >= epv->prev_dayl)
    {
        ws_flag = 1;
    }
    else
    {
        ws_flag = 0;
    }

    /* Update offset_counter and test for the end of the offset
     * period */
    if (epv->offset_flag == 1)
    {
        /* Decrement counter for offset period */
        epv->offset_counter -= (int)round (dt);

        /* If this is the end of the offset_period, reset phenology
         * flags and indices */
        if (epv->offset_counter <= 0)
        {
            epv->offset_flag = 0;
            epv->offset_counter = 0;
            epv->dormant_flag = 1;

            /* Reset the previous timestep litterfall flux memory */
            epv->prev_leafc_to_litter = 0.0;
            epv->prev_frootc_to_litter = 0.0;
        }
    }

    /* Update onset_counter and test for the end of the onset
     * period */
    if (epv->onset_flag == 1)
    {
        /* Decrement counter for onset period */
        epv->onset_counter -= (int)round (dt);

        /* If this is the end of the onset period, reset phenology
         * flags and indices */
        if (epv->onset_counter <= 0)
        {
            epv->onset_flag = 0;
            epv->onset_counter = 0;
        }
    }

    /* Test for switching from dormant period to growth period */
    if (epv->dormant_flag == 1)
    {
        /* Test to turn on growing degree-day sum, if off.
         * switch on the growing degree day sum on the winter
         * solstice */
        if (epv->onset_gddflag == 0 && ws_flag == 1)
        {
            epv->onset_gddflag = 1;
            epv->onset_gdd = 0.0;
        }

        /* Test to turn off growing degree-day sum, if on.
         * This test resets the growing degree day sum if it gets past
         * the summer solstice without reaching the threshold value.
         * In that case, it will take until the next winter solstice
         * before the growing degree-day summation starts again. */
        if (epv->onset_gddflag == 1 && ws_flag == 0)
        {
            epv->onset_gddflag = 0;
            epv->onset_gdd = 0.0;
        }

        /* If the gdd flag is set, and if the soil is above freezing
         * then accumulate growing degree days for onset trigger */
        if (epv->onset_gddflag == 1 && tsoil > 0.0)
        {
            epv->onset_gdd += tsoil * dt / DAYINSEC;
        }

        /* Set onset_flag if critical growing degree-day sum is
         * exceeded */
        if (epv->onset_gdd > onset_critsum)
        {
            epv->onset_flag = 1;
            epv->dormant_flag = 0;
            epv->onset_gddflag = 0;
            epv->onset_gdd = 0.0;
            epv->onset_counter = epc->transfer_days * DAYINSEC;
        }
    }
    /* Test for switching from growth period to offset period */
    else if (epv->offset_flag == 0)
    {
        /* Only begin to test for offset daylength once past the
         * summer sol */
        if (ws_flag == 0 && epv->dayl < critdayl)
        {
            epv->offset_flag = 1;
            epv->offset_counter = epc->litfall_days * DAYINSEC;
            epv->prev_leafc_to_litter = 0.0;
            epv->prev_frootc_to_litter = 0.0;
        }
    }
}

void OnsetGrowth (const epconst_struct *epc, const epvar_struct *epv,
    const cstate_struct *cs, cflux_struct *cf, const nstate_struct *ns,
    nflux_struct *nf, double dt)
{
    double          t1;

    if (epv->onset_flag == 1)
    {
        if (epv->onset_counter <= (int)round (dt))
        {
            t1 = 1.0 / dt;
        }
        else
        {
            t1 = 2.0 / (double)epv->onset_counter;
        }

        /* Transfer rate is defined to be a linearly decreasing
         * function that reaches zero on the last day of the transfer
         * period */
        cf->leafc_transfer_to_leafc = t1 * cs->leafc_transfer;
        nf->leafn_transfer_to_leafn = t1 * ns->leafn_transfer;
        cf->frootc_transfer_to_frootc = t1 * cs->frootc_transfer;
        nf->frootn_transfer_to_frootn = t1 * ns->frootn_transfer;
        if (epc->woody)
        {
            cf->livestemc_transfer_to_livestemc =
                t1 * cs->livestemc_transfer;
            nf->livestemn_transfer_to_livestemn =
                t1 * ns->livestemn_transfer;
            cf->deadstemc_transfer_to_deadstemc =
                t1 * cs->deadstemc_transfer;
            nf->deadstemn_transfer_to_deadstemn =
                t1 * ns->deadstemn_transfer;
            cf->livecrootc_transfer_to_livecrootc =
                t1 * cs->livecrootc_transfer;
            nf->livecrootn_transfer_to_livecrootn =
                t1 * ns->livecrootn_transfer;
            cf->deadcrootc_transfer_to_deadcrootc =
                t1 * cs->deadcrootc_transfer;
            nf->deadcrootn_transfer_to_deadcrootn =
                t1 * ns->deadcrootn_transfer;
        }
    }
}

void OffsetLitterfall (const epconst_struct *epc, epvar_struct *epv,
    const cstate_struct *cs, cflux_struct *cf, const nstate_struct *ns,
    nflux_struct *nf, double dt)
{
    double          leaflitfallc, frootlitfallc;
    double          drate;

    /* Defined such that all live material is removed by the end of the
     * litterfall period, with a linearly ramping removal rate. assumes that
     * the initial rate on the first day of litterfall is 0.0. */
    if (epv->offset_flag == 1)
    {
        if (epv->offset_counter <= (int)round (dt))
        {
            /* Last time step of litterfall, special case to gaurantee that
             * pools go to 0.0 */
            leaflitfallc = cs->leafc / dt;
            frootlitfallc = cs->frootc / dt;
        }
        else
        {
            /* Otherwise, assess litterfall rates as described above */
            leaflitfallc = epv->prev_leafc_to_litter;
            drate = 2.0 * dt *
                (cs->leafc - leaflitfallc * epv->offset_counter) /
                (epv->offset_counter * epv->offset_counter);
            leaflitfallc += drate;

            frootlitfallc = epv->prev_leafc_to_litter;
            drate = 2.0 * dt *
                (cs->frootc - frootlitfallc * epv->offset_counter) /
                (epv->offset_counter * epv->offset_counter);
            frootlitfallc += drate;
        }

        /* Leaf litterfall */
        if (leaflitfallc * dt > cs->leafc)
        {
            leaflitfallc = cs->leafc / dt;
        }
        LeafLitFall (epc, leaflitfallc, cf, nf);
        epv->prev_leafc_to_litter = leaflitfallc;

        /* Fine root litterfall */
        if (frootlitfallc * dt > cs->frootc)
        {
            frootlitfallc = cs->frootc / dt;
        }
        FRootLitFall (epc, frootlitfallc, cf, nf);
        epv->prev_frootc_to_litter = frootlitfallc;
    }
}

void BackgroundLitterfall (const epconst_struct *epc, epvar_struct *epv,
    const cstate_struct *cs, cflux_struct *cf, const nstate_struct *ns,
    nflux_struct *nf, double dt)
{
    double          leaflitfallc, frootlitfallc;

    /* Litterfall happens everyday. To prevent litterfall from driving
     * pools negative in the case of a very high mortality, fluxes are
     * checked and set to zero when the pools get too small. */

    /* Leaf litterfall */
    leaflitfallc = epv->bg_leafc_litfall_rate;
    if (leaflitfallc * dt > cs->leafc)
    {
        leaflitfallc = cs->leafc / dt;
    }
    LeafLitFall (epc, leaflitfallc, cf, nf);

    /* Fine root litterfall */
    frootlitfallc = epv->bg_frootc_litfall_rate;
    if (frootlitfallc * dt > cs->frootc)
    {
        frootlitfallc = cs->frootc / dt;
    }
    FRootLitFall (epc, frootlitfallc, cf, nf);
}

void LivewoodTurnover (const epconst_struct *epc, epvar_struct *epv,
    const cstate_struct *cs, cflux_struct *cf,
    const nstate_struct *ns, nflux_struct *nf, double dt)
{
    double          livestemtovrc, livestemtovrn;
    double          livecroottovrc, livecroottovrn;

    /* Turnover of live wood to dead wood also happens every day, at a
     * rate determined once each year, using the annual maximum livewoody
     * compartment masses and the specified livewood turnover rate */
    if (epc->woody)
    {
        /* Turnover from live stem wood to dead stem wood */
        epv->livestemc_turnover_rate =
            cs->livestemc * epc->livewood_turnover / 365.0 / DAYINSEC;
        livestemtovrc = epv->livestemc_turnover_rate;
        livestemtovrn = livestemtovrc / epc->livewood_cn;
        if (livestemtovrc * dt > cs->livestemc)
        {
            livestemtovrc = cs->livestemc / dt;
        }
        if (livestemtovrn * dt > ns->livestemn)
        {
            livestemtovrn = ns->livestemn / dt;
        }
        if (livestemtovrc && livestemtovrn)
        {
            cf->livestemc_to_deadstemc = livestemtovrc;
            nf->livestemn_to_deadstemn = livestemtovrc / epc->deadwood_cn;
            nf->livestemn_to_retransn =
                livestemtovrn - nf->livestemn_to_deadstemn;
        }

        /* Turnover from live coarse root wood to dead coarse root wood */
        epv->livecrootc_turnover_rate =
            cs->livecrootc * epc->livewood_turnover / 365.0 / DAYINSEC;
        livecroottovrc = epv->livecrootc_turnover_rate;
        livecroottovrn = livecroottovrc / epc->livewood_cn;
        if (livecroottovrc * dt > cs->livecrootc)
        {
            livecroottovrc = cs->livecrootc / dt;
        }
        if (livecroottovrn * dt > ns->livecrootn)
        {
            livecroottovrn = ns->livecrootn / dt;
        }
        if (livecroottovrc && livecroottovrn)
        {
            cf->livecrootc_to_deadcrootc = livecroottovrc;
            nf->livecrootn_to_deadcrootn =
                livecroottovrc / epc->deadwood_cn;
            nf->livecrootn_to_retransn =
                livecroottovrn - nf->livecrootn_to_deadcrootn;
        }
    }
}

void LeafLitFall (const epconst_struct *epc, double litfallc,
    cflux_struct *cf, nflux_struct *nf)
{
    double          c1, c2, c3, c4;
    double          n1, n2, n3, n4;
    double          nretrans;
    double          avg_cn;
    double          litfalln;

    avg_cn = epc->leaf_cn;
    litfalln = litfallc / epc->leaflitr_cn;

    c1 = litfallc * epc->leaflitr_flab;
    n1 = litfalln * epc->leaflitr_flab;
    c2 = litfallc * epc->leaflitr_fucel;
    n2 = litfalln * epc->leaflitr_fucel;
    c3 = litfallc * epc->leaflitr_fscel;
    n3 = litfalln * epc->leaflitr_fscel;
    c4 = litfallc * epc->leaflitr_flig;
    n4 = litfalln * epc->leaflitr_flig;
    nretrans = (litfallc / avg_cn) - (litfalln);

    /* set fluxes */
    cf->leafc_to_litr1c = c1;
    cf->leafc_to_litr2c = c2;
    cf->leafc_to_litr3c = c3;
    cf->leafc_to_litr4c = c4;
    nf->leafn_to_litr1n = n1;
    nf->leafn_to_litr2n = n2;
    nf->leafn_to_litr3n = n3;
    nf->leafn_to_litr4n = n4;
    nf->leafn_to_retransn = nretrans;
}

void FRootLitFall (const epconst_struct *epc, double litfallc,
    cflux_struct *cf, nflux_struct *nf)
{
    double          c1, c2, c3, c4;
    double          n1, n2, n3, n4;
    double          avg_cn;

    avg_cn = epc->froot_cn;

    c1 = litfallc * epc->frootlitr_flab;
    n1 = c1 / avg_cn;
    c2 = litfallc * epc->frootlitr_fucel;
    n2 = c2 / avg_cn;
    c3 = litfallc * epc->frootlitr_fscel;
    n3 = c3 / avg_cn;
    c4 = litfallc * epc->frootlitr_flig;
    n4 = c4 / avg_cn;

    /* set fluxes */
    cf->frootc_to_litr1c = c1;
    cf->frootc_to_litr2c = c2;
    cf->frootc_to_litr3c = c3;
    cf->frootc_to_litr4c = c4;
    nf->frootn_to_litr1n = n1;
    nf->frootn_to_litr2n = n2;
    nf->frootn_to_litr3n = n3;
    nf->frootn_to_litr4n = n4;
}
