#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "r2k.h"

/* Function Prototypes */
bool str_isalpha (const char *s);
bool str_islower (const char *s);
bool str_ispunct (const char *s);
bool str_isupper (const char *s);
int kana_group (const char *s);
int search_dict (char **keys, const char *key);
Dict Dict_init (void);
Dict Dict_new (void);
void Dict_free (Dict d);
void Dict_add (Dict d, const unsigned int g, const char *key, const char *val);

static bool _loaded = false;

/* Tests if all the characters in a string are letters */
bool str_isalpha (const char *s)
{
    if (s == NULL) return false;
    bool r = isalpha(s[0]);
    for (int i = 1; s[i] != '\0' && r == true; i++)
        r = isalpha(s[i]);
    return r;
}

/* Tests if all the characters in a string are lowercase */
bool str_islower (const char *s)
{
    if (s == NULL) return false;
    return !str_isupper(s);
}

/* Tests if all the characters in a string are punctuation */
bool str_ispunct (const char *s)
{
    if (s == NULL) return false;
    bool r = ispunct(s[0]);
    for (int i = 1; s[i] != '\0' && r == true; i++)
        r = ispunct(s[i]);
    return r;
}

/* Tests if any of the characters in a string is uppercase */
bool str_isupper (const char *s)
{
    if (s == NULL) return false;
    bool r = false;
    for (int i = 0; s[i] != '\0' && r == false; i++)
        r = isupper(s[i]);
    return r;
}

/* Return the group of a string to use with the conversion table */
int kana_group (const char *s)
{
    /*
     * GROUP_HIRA  => String is lowercase
     * GROUP_KATA  => String has at least one uppercase character
     * GROUP_PUNCT => String is punctuation
     */
    if (s == NULL) return -1;

    int r = -1; /* some error by default */
    if (str_isalpha(s))
        r = (str_islower(s)) ?
            GROUP_HIRA :
            GROUP_KATA;
    else if (str_ispunct(s))
        r = GROUP_PUNCT;
    return r;
}

int get_word (const Dict d, char *w, char *s, int *g, int *k)
{
    if (false
            || d == NULL
            || w == NULL
            || s == NULL
            || g == NULL
            || k == NULL
       ) return -1;
    //d = {
    //    size = { 108, 108, 5 },
    //    key = {
    //        { hira1, hira2, ... },
    //        { kata1, kata2, ... },
    //        { punct1, punct2, ... },
    //    },
    //    val = {
    //        { hira1, hira2, ... },
    //        { kata1, kata2, ... },
    //        { punct1, punct2, ... },
    //    },
    //};

    bool _dum_mask_hira[d->size[GROUP_HIRA]];
    bool _dum_mask_kata[d->size[GROUP_KATA]];
    bool _dum_mask_punct[d->size[GROUP_PUNCT]];
    bool *mask[NGROUPS] = {
        [GROUP_HIRA] = _dum_mask_hira,
        [GROUP_KATA] = _dum_mask_kata,
        [GROUP_PUNCT] = _dum_mask_punct,
    };

    int _dum_len_hira[d->size[GROUP_HIRA]];
    int _dum_len_kata[d->size[GROUP_KATA]];
    int _dum_len_punct[d->size[GROUP_PUNCT]];
    int *length[NGROUPS] = {
        [GROUP_HIRA] = _dum_len_hira,
        [GROUP_KATA] = _dum_len_kata,
        [GROUP_PUNCT] = _dum_len_punct,
    };
    for (int g = GROUP_HIRA; g < NGROUPS; g++)
        for (int k = 0; k < d->size[g]; k++) {
            mask[g][k] = true; /* Initialize mask */
            length[g][k] = strlen(d->key[g][k]);
        }

    for (int l = 0; l < 4; l++)
        for (int g = GROUP_HIRA; g < NGROUPS; g++)
            for (int k = 0; k < d->size[g]; k++) {
                if (l <= length[g][k] && s[l] != d->key[g][k][l])
                    mask[g][k] = false;
            }

    //int count[NGROUPS];
    //for (int g = GROUP_HIRA; g < NGROUPS; g++) {
    //    count[g] = 0;
    //    for (int k = 0; k < d->size[g]; k++)
    //        if (mask[g][k])
    //            count[g]++;
    //}

    return -1;
}

/* Search a dictionary for a key and return the index */
int search_dict (char **keys, const char *key)
{
    if (keys == NULL || key == NULL) return -1;
    int r = 0;
    while (keys[r] != NULL && strcmp(keys[r], key) != 0) r++;
    return (keys[r] != NULL) ? r : -1;
}

/* Convert a word to kana according to given conversion table */
char *convert_word (const Dict d, const char *s)
{
    if (d == NULL || s == NULL) return NULL;

    int g = kana_group(s);
    int i = (g < 0) ? -1 : search_dict(d->key[g], s);
    return (i < 0) ? NULL : d->val[g][i];
}

Dict Dict_new (void)
{
    Dict d = malloc(sizeof(struct Dict));

    if (false
            || d == NULL
            || (d->key[GROUP_HIRA]  = malloc(NHIRA  * sizeof(char*))) == NULL
            || (d->key[GROUP_KATA]  = malloc(NKATA  * sizeof(char*))) == NULL
            || (d->key[GROUP_PUNCT] = malloc(NPUNCT * sizeof(char*))) == NULL
            || (d->val[GROUP_HIRA]  = malloc(NHIRA  * sizeof(char*))) == NULL
            || (d->val[GROUP_KATA]  = malloc(NKATA  * sizeof(char*))) == NULL
            || (d->val[GROUP_PUNCT] = malloc(NPUNCT * sizeof(char*))) == NULL
       ) exit(EXIT_FAILURE);

    /* Initialize everything to NULL */
    for (int k = 0; k < NHIRA; k++) {
        d->key[GROUP_HIRA][k] = NULL;
        d->val[GROUP_HIRA][k] = NULL;
    }

    for (int k = 0; k < NKATA; k++) {
        d->key[GROUP_KATA][k] = NULL;
        d->val[GROUP_KATA][k] = NULL;
    }

    for (int k = 0; k < NPUNCT; k++) {
        d->key[GROUP_PUNCT][k] = NULL;
        d->val[GROUP_PUNCT][k] = NULL;
    }

    for (int g = GROUP_HIRA; g < NGROUPS; g++)
        d->size[g] = 0;

    return d;
}

void Dict_free (Dict d)
{
    if (!_loaded || d == NULL) return;

    for (int i = 0; i < 3; i++) {
        free(d->key[i]);
        free(d->val[i]);
    }
    free(d->key);
    free(d->val);
    free(d);
}

void Dict_add (Dict d, const unsigned int g, const char *key, const char *val)
{
    /* Many more cases to test here, but shouldn't happen in this context */
    if (d == NULL) return;
    const unsigned int w = d->size[g];
    const unsigned int kl = strlen(key);
    const unsigned int vl = strlen(val);

    if (false
            || (d->key[g][w] = malloc(kl * sizeof(char))) == NULL
            || (d->val[g][w] = malloc(vl * sizeof(char))) == NULL
       ) exit(EXIT_FAILURE);

    strcpy(d->key[g][w], key);
    strcpy(d->val[g][w], val);
    d->size[g]++;
}

Dict Dict_init (void)
{
    Dict d = NULL;
    if (_loaded || (d = Dict_new()) == NULL)
        return NULL;

    Dict_add(d, GROUP_HIRA, "a", HIRA_A);
    Dict_add(d, GROUP_HIRA, "i", HIRA_I);
    Dict_add(d, GROUP_HIRA, "u", HIRA_U);
    Dict_add(d, GROUP_HIRA, "e", HIRA_E);
    Dict_add(d, GROUP_HIRA, "o", HIRA_O);
    Dict_add(d, GROUP_HIRA, "ka", HIRA_KA);
    Dict_add(d, GROUP_HIRA, "ga", HIRA_GA);
    Dict_add(d, GROUP_HIRA, "ki", HIRA_KI);
    Dict_add(d, GROUP_HIRA, "gi", HIRA_GI);
    Dict_add(d, GROUP_HIRA, "ku", HIRA_KU);
    Dict_add(d, GROUP_HIRA, "gu", HIRA_GU);
    Dict_add(d, GROUP_HIRA, "ke", HIRA_KE);
    Dict_add(d, GROUP_HIRA, "ge", HIRA_GE);
    Dict_add(d, GROUP_HIRA, "ko", HIRA_KO);
    Dict_add(d, GROUP_HIRA, "go", HIRA_GO);
    Dict_add(d, GROUP_HIRA, "sa", HIRA_SA);
    Dict_add(d, GROUP_HIRA, "za", HIRA_ZA);
    Dict_add(d, GROUP_HIRA, "shi", HIRA_SHI);
    Dict_add(d, GROUP_HIRA, "ji", HIRA_JI);
    Dict_add(d, GROUP_HIRA, "su", HIRA_SU);
    Dict_add(d, GROUP_HIRA, "zu", HIRA_ZU);
    Dict_add(d, GROUP_HIRA, "se", HIRA_SE);
    Dict_add(d, GROUP_HIRA, "ze", HIRA_ZE);
    Dict_add(d, GROUP_HIRA, "so", HIRA_SO);
    Dict_add(d, GROUP_HIRA, "zo", HIRA_ZO);
    Dict_add(d, GROUP_HIRA, "ta", HIRA_TA);
    Dict_add(d, GROUP_HIRA, "da", HIRA_DA);
    Dict_add(d, GROUP_HIRA, "chi", HIRA_CHI);
    Dict_add(d, GROUP_HIRA, "dji", HIRA_DJI);
    Dict_add(d, GROUP_HIRA, "tsu", HIRA_TSU);
    Dict_add(d, GROUP_HIRA, "dzu", HIRA_DZU);
    Dict_add(d, GROUP_HIRA, "te", HIRA_TE);
    Dict_add(d, GROUP_HIRA, "de", HIRA_DE);
    Dict_add(d, GROUP_HIRA, "to", HIRA_TO);
    Dict_add(d, GROUP_HIRA, "do", HIRA_DO);
    Dict_add(d, GROUP_HIRA, "na", HIRA_NA);
    Dict_add(d, GROUP_HIRA, "ni", HIRA_NI);
    Dict_add(d, GROUP_HIRA, "nu", HIRA_NU);
    Dict_add(d, GROUP_HIRA, "ne", HIRA_NE);
    Dict_add(d, GROUP_HIRA, "no", HIRA_NO);
    Dict_add(d, GROUP_HIRA, "ha", HIRA_HA);
    Dict_add(d, GROUP_HIRA, "ba", HIRA_BA);
    Dict_add(d, GROUP_HIRA, "pa", HIRA_PA);
    Dict_add(d, GROUP_HIRA, "hi", HIRA_HI);
    Dict_add(d, GROUP_HIRA, "bi", HIRA_BI);
    Dict_add(d, GROUP_HIRA, "pi", HIRA_PI);
    Dict_add(d, GROUP_HIRA, "fu", HIRA_FU);
    Dict_add(d, GROUP_HIRA, "bu", HIRA_BU);
    Dict_add(d, GROUP_HIRA, "pu", HIRA_PU);
    Dict_add(d, GROUP_HIRA, "he", HIRA_HE);
    Dict_add(d, GROUP_HIRA, "be", HIRA_BE);
    Dict_add(d, GROUP_HIRA, "pe", HIRA_PE);
    Dict_add(d, GROUP_HIRA, "ho", HIRA_HO);
    Dict_add(d, GROUP_HIRA, "bo", HIRA_BO);
    Dict_add(d, GROUP_HIRA, "po", HIRA_PO);
    Dict_add(d, GROUP_HIRA, "ma", HIRA_MA);
    Dict_add(d, GROUP_HIRA, "mi", HIRA_MI);
    Dict_add(d, GROUP_HIRA, "mu", HIRA_MU);
    Dict_add(d, GROUP_HIRA, "me", HIRA_ME);
    Dict_add(d, GROUP_HIRA, "mo", HIRA_MO);
    Dict_add(d, GROUP_HIRA, "ya", HIRA_YA);
    Dict_add(d, GROUP_HIRA, "yu", HIRA_YU);
    Dict_add(d, GROUP_HIRA, "yo", HIRA_YO);
    Dict_add(d, GROUP_HIRA, "ra", HIRA_RA);
    Dict_add(d, GROUP_HIRA, "ri", HIRA_RI);
    Dict_add(d, GROUP_HIRA, "ru", HIRA_RU);
    Dict_add(d, GROUP_HIRA, "re", HIRA_RE);
    Dict_add(d, GROUP_HIRA, "ro", HIRA_RO);
    Dict_add(d, GROUP_HIRA, "wa", HIRA_WA);
    Dict_add(d, GROUP_HIRA, "kya", HIRA_KYA);
    Dict_add(d, GROUP_HIRA, "kyu", HIRA_KYU);
    Dict_add(d, GROUP_HIRA, "kyo", HIRA_KYO);
    Dict_add(d, GROUP_HIRA, "gya", HIRA_GYA);
    Dict_add(d, GROUP_HIRA, "gyu", HIRA_GYU);
    Dict_add(d, GROUP_HIRA, "gyo", HIRA_GYO);
    Dict_add(d, GROUP_HIRA, "sha", HIRA_SHA);
    Dict_add(d, GROUP_HIRA, "shu", HIRA_SHU);
    Dict_add(d, GROUP_HIRA, "sho", HIRA_SHO);
    Dict_add(d, GROUP_HIRA, "ja", HIRA_JA);
    Dict_add(d, GROUP_HIRA, "ju", HIRA_JU);
    Dict_add(d, GROUP_HIRA, "jo", HIRA_JO);
    Dict_add(d, GROUP_HIRA, "cha", HIRA_CHA);
    Dict_add(d, GROUP_HIRA, "chu", HIRA_CHU);
    Dict_add(d, GROUP_HIRA, "cho", HIRA_CHO);
    Dict_add(d, GROUP_HIRA, "dja", HIRA_DJA);
    Dict_add(d, GROUP_HIRA, "dju", HIRA_DJU);
    Dict_add(d, GROUP_HIRA, "djo", HIRA_DJO);
    Dict_add(d, GROUP_HIRA, "hya", HIRA_HYA);
    Dict_add(d, GROUP_HIRA, "hyu", HIRA_HYU);
    Dict_add(d, GROUP_HIRA, "hyo", HIRA_HYO);
    Dict_add(d, GROUP_HIRA, "bya", HIRA_BYA);
    Dict_add(d, GROUP_HIRA, "byu", HIRA_BYU);
    Dict_add(d, GROUP_HIRA, "byo", HIRA_BYO);
    Dict_add(d, GROUP_HIRA, "pya", HIRA_PYA);
    Dict_add(d, GROUP_HIRA, "pyu", HIRA_PYU);
    Dict_add(d, GROUP_HIRA, "pyo", HIRA_PYO);
    Dict_add(d, GROUP_HIRA, "nya", HIRA_NYA);
    Dict_add(d, GROUP_HIRA, "nyu", HIRA_NYU);
    Dict_add(d, GROUP_HIRA, "nyo", HIRA_NYO);
    Dict_add(d, GROUP_HIRA, "wi", HIRA_WI);
    Dict_add(d, GROUP_HIRA, "we", HIRA_WE);
    Dict_add(d, GROUP_HIRA, "wo", HIRA_WO);
    Dict_add(d, GROUP_HIRA, "n", HIRA_N);
    Dict_add(d, GROUP_HIRA, "vu", HIRA_VU);

    Dict_add(d, GROUP_KATA, "A", KATA_A);
    Dict_add(d, GROUP_KATA, "I", KATA_I);
    Dict_add(d, GROUP_KATA, "U", KATA_U);
    Dict_add(d, GROUP_KATA, "E", KATA_E);
    Dict_add(d, GROUP_KATA, "O", KATA_O);
    Dict_add(d, GROUP_KATA, "KA", KATA_KA);
    Dict_add(d, GROUP_KATA, "GA", KATA_GA);
    Dict_add(d, GROUP_KATA, "KI", KATA_KI);
    Dict_add(d, GROUP_KATA, "GI", KATA_GI);
    Dict_add(d, GROUP_KATA, "KU", KATA_KU);
    Dict_add(d, GROUP_KATA, "GU", KATA_GU);
    Dict_add(d, GROUP_KATA, "KE", KATA_KE);
    Dict_add(d, GROUP_KATA, "GE", KATA_GE);
    Dict_add(d, GROUP_KATA, "KO", KATA_KO);
    Dict_add(d, GROUP_KATA, "GO", KATA_GO);
    Dict_add(d, GROUP_KATA, "SA", KATA_SA);
    Dict_add(d, GROUP_KATA, "ZA", KATA_ZA);
    Dict_add(d, GROUP_KATA, "SHI", KATA_SHI);
    Dict_add(d, GROUP_KATA, "JI", KATA_JI);
    Dict_add(d, GROUP_KATA, "SU", KATA_SU);
    Dict_add(d, GROUP_KATA, "ZU", KATA_ZU);
    Dict_add(d, GROUP_KATA, "SE", KATA_SE);
    Dict_add(d, GROUP_KATA, "ZE", KATA_ZE);
    Dict_add(d, GROUP_KATA, "SO", KATA_SO);
    Dict_add(d, GROUP_KATA, "ZO", KATA_ZO);
    Dict_add(d, GROUP_KATA, "TA", KATA_TA);
    Dict_add(d, GROUP_KATA, "DA", KATA_DA);
    Dict_add(d, GROUP_KATA, "CHI", KATA_CHI);
    Dict_add(d, GROUP_KATA, "DJI", KATA_DJI);
    Dict_add(d, GROUP_KATA, "TSU", KATA_TSU);
    Dict_add(d, GROUP_KATA, "DZU", KATA_DZU);
    Dict_add(d, GROUP_KATA, "TE", KATA_TE);
    Dict_add(d, GROUP_KATA, "DE", KATA_DE);
    Dict_add(d, GROUP_KATA, "TO", KATA_TO);
    Dict_add(d, GROUP_KATA, "DO", KATA_DO);
    Dict_add(d, GROUP_KATA, "NA", KATA_NA);
    Dict_add(d, GROUP_KATA, "NI", KATA_NI);
    Dict_add(d, GROUP_KATA, "NU", KATA_NU);
    Dict_add(d, GROUP_KATA, "NE", KATA_NE);
    Dict_add(d, GROUP_KATA, "NO", KATA_NO);
    Dict_add(d, GROUP_KATA, "HA", KATA_HA);
    Dict_add(d, GROUP_KATA, "BA", KATA_BA);
    Dict_add(d, GROUP_KATA, "PA", KATA_PA);
    Dict_add(d, GROUP_KATA, "HI", KATA_HI);
    Dict_add(d, GROUP_KATA, "BI", KATA_BI);
    Dict_add(d, GROUP_KATA, "PI", KATA_PI);
    Dict_add(d, GROUP_KATA, "FU", KATA_FU);
    Dict_add(d, GROUP_KATA, "BU", KATA_BU);
    Dict_add(d, GROUP_KATA, "PU", KATA_PU);
    Dict_add(d, GROUP_KATA, "HE", KATA_HE);
    Dict_add(d, GROUP_KATA, "BE", KATA_BE);
    Dict_add(d, GROUP_KATA, "PE", KATA_PE);
    Dict_add(d, GROUP_KATA, "HO", KATA_HO);
    Dict_add(d, GROUP_KATA, "BO", KATA_BO);
    Dict_add(d, GROUP_KATA, "PO", KATA_PO);
    Dict_add(d, GROUP_KATA, "MA", KATA_MA);
    Dict_add(d, GROUP_KATA, "MI", KATA_MI);
    Dict_add(d, GROUP_KATA, "MU", KATA_MU);
    Dict_add(d, GROUP_KATA, "ME", KATA_ME);
    Dict_add(d, GROUP_KATA, "MO", KATA_MO);
    Dict_add(d, GROUP_KATA, "YA", KATA_YA);
    Dict_add(d, GROUP_KATA, "YU", KATA_YU);
    Dict_add(d, GROUP_KATA, "YO", KATA_YO);
    Dict_add(d, GROUP_KATA, "RA", KATA_RA);
    Dict_add(d, GROUP_KATA, "RI", KATA_RI);
    Dict_add(d, GROUP_KATA, "RU", KATA_RU);
    Dict_add(d, GROUP_KATA, "RE", KATA_RE);
    Dict_add(d, GROUP_KATA, "RO", KATA_RO);
    Dict_add(d, GROUP_KATA, "WA", KATA_WA);
    Dict_add(d, GROUP_KATA, "KYA", KATA_KYA);
    Dict_add(d, GROUP_KATA, "KYU", KATA_KYU);
    Dict_add(d, GROUP_KATA, "KYO", KATA_KYO);
    Dict_add(d, GROUP_KATA, "GYA", KATA_GYA);
    Dict_add(d, GROUP_KATA, "GYU", KATA_GYU);
    Dict_add(d, GROUP_KATA, "GYO", KATA_GYO);
    Dict_add(d, GROUP_KATA, "SHA", KATA_SHA);
    Dict_add(d, GROUP_KATA, "SHU", KATA_SHU);
    Dict_add(d, GROUP_KATA, "SHO", KATA_SHO);
    Dict_add(d, GROUP_KATA, "JA",  KATA_JA);
    Dict_add(d, GROUP_KATA, "JU",  KATA_JU);
    Dict_add(d, GROUP_KATA, "JO",  KATA_JO);
    Dict_add(d, GROUP_KATA, "CHA", KATA_CHA);
    Dict_add(d, GROUP_KATA, "CHU", KATA_CHU);
    Dict_add(d, GROUP_KATA, "CHO", KATA_CHO);
    Dict_add(d, GROUP_KATA, "DJA", KATA_DJA);
    Dict_add(d, GROUP_KATA, "DJU", KATA_DJU);
    Dict_add(d, GROUP_KATA, "DJO", KATA_DJO);
    Dict_add(d, GROUP_KATA, "HYA", KATA_HYA);
    Dict_add(d, GROUP_KATA, "HYU", KATA_HYU);
    Dict_add(d, GROUP_KATA, "HYO", KATA_HYO);
    Dict_add(d, GROUP_KATA, "BYA", KATA_BYA);
    Dict_add(d, GROUP_KATA, "BYU", KATA_BYU);
    Dict_add(d, GROUP_KATA, "BYO", KATA_BYO);
    Dict_add(d, GROUP_KATA, "PYA", KATA_PYA);
    Dict_add(d, GROUP_KATA, "PYU", KATA_PYU);
    Dict_add(d, GROUP_KATA, "PYO", KATA_PYO);
    Dict_add(d, GROUP_KATA, "NYA", KATA_NYA);
    Dict_add(d, GROUP_KATA, "NYU", KATA_NYU);
    Dict_add(d, GROUP_KATA, "NYO", KATA_NYO);
    Dict_add(d, GROUP_KATA, "WI", KATA_WI);
    Dict_add(d, GROUP_KATA, "WE", KATA_WE);
    Dict_add(d, GROUP_KATA, "WO", KATA_WO);
    Dict_add(d, GROUP_KATA, "N", KATA_N);
    Dict_add(d, GROUP_KATA, "VU", KATA_VU);

    Dict_add(d, GROUP_PUNCT, ".", PUNCT_PERIOD);
    Dict_add(d, GROUP_PUNCT, ",", PUNCT_COMMA);
    Dict_add(d, GROUP_PUNCT, "!", PUNCT_EMARK);
    Dict_add(d, GROUP_PUNCT, "?", PUNCT_QMARK);

    _loaded = true;

    return d;
}

#ifdef _R2K_STANDALONE
int main (int argc, char *argv[])
{
    int i = 0;
    Dict d = Dict_init();

    for (i = 0; i < argc; i++)
        /* process(argv[i]); */
        printf("%s\n", argv[i]);

    for (i = 0; false
            || i < d->size[GROUP_HIRA]
            || i < d->size[GROUP_KATA]
            || i < d->size[GROUP_PUNCT];
            i++)
    {
        if (i < d->size[GROUP_HIRA]) printf("%s\t%s\t", d->key[GROUP_HIRA][i], d->val[GROUP_HIRA][i]);
        if (i < d->size[GROUP_KATA]) printf("%s\t%s\t", d->key[GROUP_KATA][i], d->val[GROUP_KATA][i]);
        if (i < d->size[GROUP_PUNCT]) printf("%s\t%s", d->key[GROUP_PUNCT][i], d->val[GROUP_PUNCT][i]);
        putchar('\n');
    }

    exit(
            (argc - i == 0) ?
            EXIT_SUCCESS :
            EXIT_FAILURE
        );
}
#endif /* _R2K_STANDALONE */
