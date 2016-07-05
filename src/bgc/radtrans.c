/* 
 * radtrans.c
 * calculate leaf area index, sun and shade fractions, and specific
 * leaf area for sun and shade canopy fractions, then calculate
 * canopy radiation interception and transmission 
 * 
 * *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 * Biome-BGC version 4.2 (final release)
 * See copyright.txt for Copyright information
 * *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */

#include "pihm.h"

void RadTrans (const cstate_struct *cs, eflux_struct *ef, pstate_struct *ps, const epconst_struct *epc, epvar_struct *epv)
{
    /* calculate the projected leaf area and SLA for sun and shade fractions
     * and the canopy transmission and absorption of shortwave radiation
     * based on the Beer's Law assumption of radiation attenuation as a 
     * function of projected LAI.
     */

    double          proj_lai;
    double          albedo_sw, albedo_par;
    double          sw, par;
    double          swabs, swtrans;
    double          parabs;
    double          k;
    double          k_sw, k_par;
    double          swabs_plaisun, swabs_plaishade;
    double          swabs_per_plaisun, swabs_per_plaishade;
    double          parabs_plaisun, parabs_plaishade;
    double          parabs_per_plaisun, parabs_per_plaishade;

    /* The following equations estimate the albedo and extinction 
     * coefficients for the shortwave and PAR spectra from the values given for the
     * entire shortwave range (from Jones, H.G., 1992. Plants and Microclimate,
     * 2nd  Edition. Cambridge University Press. pp. 30-38.) These conversions
     * are approximated from the information given in Jones.
     */

    if (cs->leafc > 0.0)
    {
        /* Calculate whole-canopy projected and all-sided LAI */
        epv->proj_lai = cs->leafc * epc->avg_proj_sla;
        epv->all_lai = epv->proj_lai * epc->lai_ratio;

        /* Calculate projected LAI for sunlit and shaded canopy portions */
        epv->plaisun = 1.0 - exp (-epv->proj_lai);
        epv->plaishade = epv->proj_lai - epv->plaisun;
        if (epv->plaishade < 0.0)
        {
            printf ("FATAL ERROR: Negative plaishade\n");
            printf ("LAI of shaded canopy = %lf\n", epv->plaishade);
            exit (1);
        }

        /* calculate the projected specific leaf area for sunlit and 
         * shaded canopy fractions */
        epv->sun_proj_sla = (epv->plaisun + (epv->plaishade / epc->sla_ratio)) / cs->leafc;
        epv->shade_proj_sla = epv->sun_proj_sla * epc->sla_ratio;
    }
    else if (cs->leafc == 0.0)
    {
        epv->all_lai = 0.0;
        epv->proj_lai = 0.0;
        epv->plaisun = 0.0;
        epv->plaishade = 0.0;
        epv->sun_proj_sla = 0.0;
        epv->shade_proj_sla = 0.0;
    }
    else
    {
        printf ("FATAL ERROR: Negative leaf carbon pool\n");
        printf ("leafc = %.7e\n", cs->leafc);
        exit (1);
    }

    k = epc->ext_coef;
    proj_lai = epv->proj_lai;

    /* calculate total shortwave absorbed */
    k_sw = k;
    albedo_sw = ps->albedo;

    /* FIXED 02/05/04 */
    if (ef->soldn < 0.0)
    {
        ef->soldn = 0.0;
    }

    sw = ef->soldn * (1.0 - albedo_sw);
    swabs = sw * (1.0 - exp (-k_sw * proj_lai));
    //    printf ("leafc = %lf, sw = %lf, k_sw = %lf, proj_lai = %lf, swabs = %lf\n", cs->leafc, sw, k_sw, proj_lai, swabs);
    swtrans = sw - swabs;

    /* calculate PAR absorbed */
    k_par = k * 1.0;
    albedo_par = ps->albedo / 3.0;

    /* FIXED 02/05/04 */
    if (ef->par < 0.0)
    {
        ef->par = 0.0;
    }

    par = ef->par * (1.0 - albedo_par);
    parabs = par * (1.0 - exp (-k_par * proj_lai));

    /* calculate the total shortwave absorbed by the sunlit and
     * shaded canopy fractions */
    swabs_plaisun = k_sw * sw * epv->plaisun;
    swabs_plaishade = swabs - swabs_plaisun;

    /* FIXED 02/05/04 */
    if (swabs_plaishade < 0.0)
    {
        swabs_plaisun = swabs;
        swabs_plaishade = 0;
    }

    /* convert this to the shortwave absorbed per unit LAI in the sunlit and 
     * shaded canopy fractions */
    if (proj_lai > 0.0)
    {
        swabs_per_plaisun = swabs_plaisun / epv->plaisun;
        swabs_per_plaishade = swabs_plaishade / epv->plaishade;
    }
    else
    {
        swabs_per_plaisun = swabs_per_plaishade = 0.0;
    }

    /* calculate the total PAR absorbed by the sunlit and
     * shaded canopy fractions */
    parabs_plaisun = k_par * par * epv->plaisun;
    parabs_plaishade = parabs - parabs_plaisun;

    /* FIXED 02/05/04 */
    if (parabs_plaishade < 0.0)
    {
        parabs_plaisun = parabs;
        parabs_plaishade = 0.0;
    }

    /* convert this to the PAR absorbed per unit LAI in the sunlit and 
     * shaded canopy fractions */
    if (proj_lai > 0.0)
    {
        parabs_per_plaisun = parabs_plaisun / epv->plaisun;
        parabs_per_plaishade = parabs_plaishade / epv->plaishade;
    }
    else
    {
        parabs_per_plaisun = parabs_per_plaishade = 0.0;
    }

    /* assign structure values */
    ef->swabs = swabs;
    ef->swtrans = swtrans;
    ef->swabs_per_plaisun = swabs_per_plaisun;
    ef->swabs_per_plaishade = swabs_per_plaishade;
    /* calculate PPFD: assumes an average energy for PAR photon (EPAR, umol/J)
     * unit conversion: W/m2 --> umol/m2/s. */
    ps->ppfd_per_plaisun = parabs_per_plaisun * EPAR;
    ps->ppfd_per_plaishade = parabs_per_plaishade * EPAR;
    ef->parabs = parabs;
}
