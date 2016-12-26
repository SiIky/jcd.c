#ifndef _R2K_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bool.h"

#define _R2K_STANDALONE 1

/* Conversion Table */
#define GROUP_HIRA      0
#define GROUP_KATA      1
#define GROUP_PUNCT     2
#define NGROUPS         3

/* Hiragana */
#define NHIRA   (108 - 3) /* Number of lines minus 3 for the small y-row */
const char HIRA_A[]          = "\u3042"; /* あ */
const char HIRA_I[]          = "\u3044"; /* い */
const char HIRA_U[]          = "\u3046"; /* う */
const char HIRA_E[]          = "\u3048"; /* え */
const char HIRA_O[]          = "\u304a"; /* お */
const char HIRA_KA[]         = "\u304b"; /* か */
const char HIRA_GA[]         = "\u304c"; /* が */
const char HIRA_KI[]         = "\u304d"; /* き */
const char HIRA_GI[]         = "\u304e"; /* ぎ */
const char HIRA_KU[]         = "\u304f"; /* く */
const char HIRA_GU[]         = "\u3050"; /* ぐ */
const char HIRA_KE[]         = "\u3051"; /* け */
const char HIRA_GE[]         = "\u3052"; /* げ */
const char HIRA_KO[]         = "\u3053"; /* こ */
const char HIRA_GO[]         = "\u3054"; /* ご */
const char HIRA_SA[]         = "\u3055"; /* さ */
const char HIRA_ZA[]         = "\u3056"; /* ざ */
const char HIRA_SHI[]        = "\u3057"; /* し */
const char HIRA_JI[]         = "\u3058"; /* じ */
const char HIRA_SU[]         = "\u3059"; /* す */
const char HIRA_ZU[]         = "\u305a"; /* ず */
const char HIRA_SE[]         = "\u305b"; /* せ */
const char HIRA_ZE[]         = "\u305c"; /* ぜ */
const char HIRA_SO[]         = "\u305d"; /* そ */
const char HIRA_ZO[]         = "\u305e"; /* ぞ */
const char HIRA_TA[]         = "\u305f"; /* た */
const char HIRA_DA[]         = "\u3060"; /* だ */
const char HIRA_CHI[]        = "\u3061"; /* ち */
const char HIRA_DJI[]        = "\u3062"; /* ぢ */
const char HIRA_TSU[]        = "\u3064"; /* つ */
const char HIRA_DZU[]        = "\u3065"; /* づ */
const char HIRA_TE[]         = "\u3066"; /* て */
const char HIRA_DE[]         = "\u3067"; /* で */
const char HIRA_TO[]         = "\u3068"; /* と */
const char HIRA_DO[]         = "\u3069"; /* ど */
const char HIRA_NA[]         = "\u306a"; /* な */
const char HIRA_NI[]         = "\u306b"; /* に */
const char HIRA_NU[]         = "\u306c"; /* ぬ */
const char HIRA_NE[]         = "\u306d"; /* ね */
const char HIRA_NO[]         = "\u306d"; /* の */
const char HIRA_HA[]         = "\u306f"; /* は */
const char HIRA_BA[]         = "\u3070"; /* ば */
const char HIRA_PA[]         = "\u3071"; /* ぱ */
const char HIRA_HI[]         = "\u3072"; /* ひ */
const char HIRA_BI[]         = "\u3073"; /* び */
const char HIRA_PI[]         = "\u3074"; /* ぴ */
const char HIRA_FU[]         = "\u3075"; /* ふ */
const char HIRA_BU[]         = "\u3076"; /* ぶ */
const char HIRA_PU[]         = "\u3077"; /* ぷ */
const char HIRA_HE[]         = "\u3078"; /* へ */
const char HIRA_BE[]         = "\u3079"; /* べ */
const char HIRA_PE[]         = "\u307a"; /* ぺ */
const char HIRA_HO[]         = "\u307b"; /* ほ */
const char HIRA_BO[]         = "\u307c"; /* ぼ */
const char HIRA_PO[]         = "\u307d"; /* ぽ */
const char HIRA_MA[]         = "\u307e"; /* ま */
const char HIRA_MI[]         = "\u307f"; /* み */
const char HIRA_MU[]         = "\u3080"; /* む */
const char HIRA_ME[]         = "\u3081"; /* め */
const char HIRA_MO[]         = "\u3082"; /* も */
const char HIRA_SMALL_YA[]   = "\u3083"; /* ゃ */
const char HIRA_YA[]         = "\u3084"; /* や */
const char HIRA_SMALL_YU[]   = "\u3085"; /* ゅ */
const char HIRA_YU[]         = "\u3086"; /* ゆ */
const char HIRA_SMALL_YO[]   = "\u3087"; /* ょ */
const char HIRA_YO[]         = "\u3088"; /* よ */
const char HIRA_RA[]         = "\u3089"; /* ら */
const char HIRA_RI[]         = "\u308a"; /* り */
const char HIRA_RU[]         = "\u308b"; /* る */
const char HIRA_RE[]         = "\u308c"; /* れ */
const char HIRA_RO[]         = "\u308d"; /* ろ */
const char HIRA_WA[]         = "\u308f"; /* わ */
const char HIRA_KYA[]        = "\u304d\u3083"; /* き */
const char HIRA_KYU[]        = "\u304d\u3085"; /* き */
const char HIRA_KYO[]        = "\u304d\u3087"; /* き */
const char HIRA_GYA[]        = "\u304e\u3083"; /*  */
const char HIRA_GYU[]        = "\u304e\u3085"; /*  */
const char HIRA_GYO[]        = "\u304e\u3087"; /*  */
const char HIRA_SHA[]        = "\u3057\u3083"; /*  */
const char HIRA_SHU[]        = "\u3057\u3085"; /*  */
const char HIRA_SHO[]        = "\u3057\u3087"; /*  */
const char HIRA_JA[]         = "\u3058\u3083"; /*  */
const char HIRA_JU[]         = "\u3058\u3085"; /*  */
const char HIRA_JO[]         = "\u3058\u3087"; /*  */
const char HIRA_CHA[]        = "\u3061\u3083"; /*  */
const char HIRA_CHU[]        = "\u3061\u3085"; /*  */
const char HIRA_CHO[]        = "\u3061\u3087"; /*  */
const char HIRA_DJA[]        = "\u3062\u3083"; /*  */
const char HIRA_DJU[]        = "\u3062\u3085"; /*  */
const char HIRA_DJO[]        = "\u3062\u3087"; /*  */
const char HIRA_HYA[]        = "\u3072\u3083"; /*  */
const char HIRA_HYU[]        = "\u3072\u3085"; /*  */
const char HIRA_HYO[]        = "\u3072\u3087"; /*  */
const char HIRA_BYA[]        = "\u3073\u3083"; /*  */
const char HIRA_BYU[]        = "\u3073\u3085"; /*  */
const char HIRA_BYO[]        = "\u3073\u3087"; /*  */
const char HIRA_PYA[]        = "\u3074\u3083"; /*  */
const char HIRA_PYU[]        = "\u3074\u3085"; /*  */
const char HIRA_PYO[]        = "\u3074\u3087"; /*  */
const char HIRA_NYA[]        = "\u306b\u3083"; /*  */
const char HIRA_NYU[]        = "\u306b\u3085"; /*  */
const char HIRA_NYO[]        = "\u306b\u3087"; /*  */
const char HIRA_WI[]         = "\u3090"; /* ゐ */
const char HIRA_WE[]         = "\u3091"; /* ゑ */
const char HIRA_WO[]         = "\u3092"; /* を */
const char HIRA_N[]          = "\u3093"; /* ん */
const char HIRA_VU[]         = "\u3094"; /* ゔ */

/* Katakana */
#define NKATA   (108 - 3) /* Number of lines minus 3 for the small y-row */
const char KATA_A[]          = "\u30a2"; /* ア */
const char KATA_I[]          = "\u30a4"; /* イ */
const char KATA_U[]          = "\u30a6"; /* ウ */
const char KATA_E[]          = "\u30a8"; /* エ */
const char KATA_O[]          = "\u30aa"; /* オ */
const char KATA_KA[]         = "\u30ab"; /* カ */
const char KATA_GA[]         = "\u30ac"; /* ガ */
const char KATA_KI[]         = "\u30ad"; /* キ */
const char KATA_GI[]         = "\u30ae"; /* ギ */
const char KATA_KU[]         = "\u30af"; /* ク */
const char KATA_GU[]         = "\u30b0"; /* グ */
const char KATA_KE[]         = "\u30b1"; /* ケ */
const char KATA_GE[]         = "\u30b2"; /* ゲ */
const char KATA_KO[]         = "\u30b3"; /* コ */
const char KATA_GO[]         = "\u30b4"; /* ゴ */
const char KATA_SA[]         = "\u30b5"; /* サ */
const char KATA_ZA[]         = "\u30b6"; /* ザ */
const char KATA_SHI[]        = "\u30b7"; /* シ */
const char KATA_JI[]         = "\u30b8"; /* ジ */
const char KATA_SU[]         = "\u30b9"; /* ス */
const char KATA_ZU[]         = "\u30ba"; /* ズ */
const char KATA_SE[]         = "\u30bb"; /* セ */
const char KATA_ZE[]         = "\u30bc"; /* ゼ */
const char KATA_SO[]         = "\u30bd"; /* ソ */
const char KATA_ZO[]         = "\u30be"; /* ゾ */
const char KATA_TA[]         = "\u30bf"; /* タ */
const char KATA_DA[]         = "\u30c0"; /* ダ */
const char KATA_CHI[]        = "\u30c1"; /* チ */
const char KATA_DJI[]        = "\u30c2"; /* ヂ */
const char KATA_TSU[]        = "\u30c4"; /* ツ */
const char KATA_DZU[]        = "\u30c5"; /* ヅ */
const char KATA_TE[]         = "\u30c6"; /* テ */
const char KATA_DE[]         = "\u30c7"; /* デ */
const char KATA_TO[]         = "\u30c8"; /* ト */
const char KATA_DO[]         = "\u30c9"; /* ド */
const char KATA_NA[]         = "\u30ca"; /* ナ */
const char KATA_NI[]         = "\u30cb"; /* ニ */
const char KATA_NU[]         = "\u30cc"; /* ヌ */
const char KATA_NE[]         = "\u30cd"; /* ネ */
const char KATA_NO[]         = "\u30ce"; /* ノ */
const char KATA_HA[]         = "\u30cf"; /* ハ */
const char KATA_BA[]         = "\u30d0"; /* バ */
const char KATA_PA[]         = "\u30d1"; /* パ */
const char KATA_HI[]         = "\u30d2"; /* ヒ */
const char KATA_BI[]         = "\u30d3"; /* ビ */
const char KATA_PI[]         = "\u30d4"; /* ピ */
const char KATA_FU[]         = "\u30d5"; /* フ */
const char KATA_BU[]         = "\u30d6"; /* ブ */
const char KATA_PU[]         = "\u30d7"; /* プ */
const char KATA_HE[]         = "\u30d8"; /* ヘ */
const char KATA_BE[]         = "\u30d9"; /* ベ */
const char KATA_PE[]         = "\u30da"; /* ペ */
const char KATA_HO[]         = "\u30db"; /* ホ */
const char KATA_BO[]         = "\u30dc"; /* ボ */
const char KATA_PO[]         = "\u30dd"; /* ポ */
const char KATA_MA[]         = "\u30de"; /* マ */
const char KATA_MI[]         = "\u30df"; /* ミ */
const char KATA_MU[]         = "\u30e0"; /* ム */
const char KATA_ME[]         = "\u30e1"; /* メ */
const char KATA_MO[]         = "\u30e2"; /* モ */
const char KATA_SMALL_YA[]   = "\u30e3"; /* ャ */
const char KATA_YA[]         = "\u30e4"; /* ヤ */
const char KATA_SMALL_YU[]   = "\u30e5"; /* ュ */
const char KATA_YU[]         = "\u30e6"; /* ユ */
const char KATA_SMALL_YO[]   = "\u30e7"; /* ョ */
const char KATA_YO[]         = "\u30e8"; /* ヨ */
const char KATA_RA[]         = "\u30e9"; /* ラ */
const char KATA_RI[]         = "\u30ea"; /* リ */
const char KATA_RU[]         = "\u30eb"; /* ル */
const char KATA_RE[]         = "\u30ec"; /* レ */
const char KATA_RO[]         = "\u30ed"; /* ロ */
const char KATA_WA[]         = "\u30ef"; /* ワ */
const char KATA_KYA[]        = "\u30ad\u30e3"; /*  */
const char KATA_KYU[]        = "\u30ad\u30e5"; /*  */
const char KATA_KYO[]        = "\u30ad\u30e7"; /*  */
const char KATA_GYA[]        = "\u30ae\u30e3"; /*  */
const char KATA_GYU[]        = "\u30ae\u30e5"; /*  */
const char KATA_GYO[]        = "\u30ae\u30e7"; /*  */
const char KATA_SHA[]        = "\u30b7\u30e3"; /*  */
const char KATA_SHU[]        = "\u30b7\u30e5"; /*  */
const char KATA_SHO[]        = "\u30b7\u30e7"; /*  */
const char KATA_JA[]         = "\u30b8\u30e3"; /*  */
const char KATA_JU[]         = "\u30b8\u30e5"; /*  */
const char KATA_JO[]         = "\u30b8\u30e7"; /*  */
const char KATA_CHA[]        = "\u30c1\u30e3"; /*  */
const char KATA_CHU[]        = "\u30c1\u30e5"; /*  */
const char KATA_CHO[]        = "\u30c1\u30e7"; /*  */
const char KATA_DJA[]        = "\u30c2\u30e3"; /*  */
const char KATA_DJU[]        = "\u30c2\u30e5"; /*  */
const char KATA_DJO[]        = "\u30c2\u30e7"; /*  */
const char KATA_HYA[]        = "\u30d2\u30e3"; /*  */
const char KATA_HYU[]        = "\u30d2\u30e5"; /*  */
const char KATA_HYO[]        = "\u30d2\u30e7"; /*  */
const char KATA_BYA[]        = "\u30d3\u30e3"; /*  */
const char KATA_BYU[]        = "\u30d3\u30e5"; /*  */
const char KATA_BYO[]        = "\u30d3\u30e7"; /*  */
const char KATA_PYA[]        = "\u30d4\u30e3"; /*  */
const char KATA_PYU[]        = "\u30d4\u30e5"; /*  */
const char KATA_PYO[]        = "\u30d4\u30e7"; /*  */
const char KATA_NYA[]        = "\u30cb\u30e3"; /*  */
const char KATA_NYU[]        = "\u30cb\u30e5"; /*  */
const char KATA_NYO[]        = "\u30cb\u30e7"; /*  */
const char KATA_WI[]         = "\u30f0"; /* ヰ */
const char KATA_WE[]         = "\u30f1"; /* ヱ */
const char KATA_WO[]         = "\u30f2"; /* ヲ */
const char KATA_N[]          = "\u30f3"; /* ン */
const char KATA_VU[]         = "\u30f4"; /* ヴ */

/* Punctuation */
#define NPUNCT  5
const char PUNCT_PERIOD[]    = "\u3002"; /* 。 */
const char PUNCT_COMMA[]     = "\u3001"; /* 、 */
const char PUNCT_EMARK[]     = "\uff01"; /* ！ */
const char PUNCT_QMARK[]     = "\uff1f"; /* ？ */

typedef struct Dict {
    short size[NGROUPS];
    char **key[NGROUPS];
    char **val[NGROUPS];
} *Dict;

/* Function Prototypes */
char *convert_word (const Dict t, const char *s);

#endif /* _R2K_H */
