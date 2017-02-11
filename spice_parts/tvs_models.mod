**************************************************
** Littelfuse AK10-xxxC, TVS Diode	    **
**                                                  	    **
** Jifeng Z.                                  	    **
** Wuxi Technical Center             	    **
** Littelfuse, Inc                          	    **
**                                                  	    **
** Model Generated: B, 10/22/2013   	    **
** MODEL FORMAT: SPICE3     	    **
**************************************************
.SUBCKT AK10-030C  1  2
*       TERMINALS: T1 T2
D1p     1          3  Dtvs
D1n     4          3  Dtvs
D2p     4          5  Dtvs
D2n     6          5  Dtvs
D3p     6          7  Dtvs
D3n     2          7  Dtvs
Rleak   1          2  3.0meg
.MODEL  Dtvs       D  (IS=1.0e-5 RS=0.000426 N=1.5 IBV=10m BV=10.77 CJO=35000p)
.ENDS

.SUBCKT AK10-058C  1  2
*       TERMINALS: T1 T2
D1p     1          3  Dtvs
D1n     4          3  Dtvs
D2p     4          5  Dtvs
D2n     6          5  Dtvs
D3p     6          7  Dtvs
D3n     8          7  Dtvs
D4p     8          9  Dtvs
D4n     2          9  Dtvs
Rleak   1          2  5.8meg
.MODEL  Dtvs       D  (IS=1.0e-5 RS=0.000426 N=1.5 IBV=10m BV=16.16 CJO=35000p)
.ENDS

.SUBCKT AK10-066C  1  2
*       TERMINALS: T1 T2
D1p     1          3  Dtvs
D1n     4          3  Dtvs
D2p     4          5  Dtvs
D2n     6          5  Dtvs
D3p     6          7  Dtvs
D3n     8          7  Dtvs
D4p     8          9  Dtvs
D4n     10         9  Dtvs
D5p     10         11 Dtvs
D5n     2         11 Dtvs
Rleak   1          2  6.6meg
.MODEL  Dtvs       D  (IS=1.0e-5 RS=0.000329 N=1.5 IBV=10m BV=14.50 CJO=35000p)
.ENDS

.SUBCKT AK10-076C  1  2
*       TERMINALS: T1 T2
D1p     1          3  Dtvs
D1n     4          3  Dtvs
D2p     4          5  Dtvs
D2n     6          5  Dtvs
D3p     6          7  Dtvs
D3n     8          7  Dtvs
D4p     8          9  Dtvs
D4n     10         9  Dtvs
D5p     10         11 Dtvs
D5n     12         11 Dtvs
D6p     12         13 Dtvs
D6n     2          13 Dtvs
Rleak   1          2  7.6meg
.MODEL  Dtvs       D  (IS=1.0e-5 RS=0.000329 N=1.5 IBV=10m BV=14.31 CJO=35000p)
.ENDS

.SUBCKT AK10-170C  1  2
*       TERMINALS: T1 T2
D1p     1          3  Dtvs
D1n     4          3  Dtvs
D2p     4          5  Dtvs
D2n     6          5  Dtvs
D3p     6          7  Dtvs
D3n     8          7  Dtvs
D4p     8          9  Dtvs
D4n     10         9  Dtvs
D5p     10         11 Dtvs
D5n     12         11 Dtvs
D6p     12         13 Dtvs
D6n     14         13 Dtvs
D7p     14         15 Dtvs
D7n     16         15 Dtvs
D8p     16         17 Dtvs
D8n     18         17 Dtvs
D9p     18         19 Dtvs
D9n     20         19 Dtvs
D10p    20         21 Dtvs
D10n    22         21 Dtvs
D11p    22         23 Dtvs
D11n    24         23 Dtvs
D12p    24         25 Dtvs
D12n    26         25 Dtvs
D13p    26         27 Dtvs
D13n    28         27 Dtvs
D14p    28         29 Dtvs
D14n    2          29 Dtvs
Rleak   1          2  17.0meg
.MODEL  Dtvs       D  (IS=1.0e-5 RS=0.000175 N=1.5 IBV=10m BV=12.99 CJO=35000p)
.ENDS

.SUBCKT AK10-190C  1  2
*       TERMINALS: T1 T2
D1p     1          3  Dtvs
D1n     4          3  Dtvs
D2p     4          5  Dtvs
D2n     6          5  Dtvs
D3p     6          7  Dtvs
D3n     8          7  Dtvs
D4p     8          9  Dtvs
D4n     10         9  Dtvs
D5p     10         11 Dtvs
D5n     12         11 Dtvs
D6p     12         13 Dtvs
D6n     14         13 Dtvs
D7p     14         15 Dtvs
D7n     16         15 Dtvs
D8p     16         17 Dtvs
D8n     18         17 Dtvs
D9p     18         19 Dtvs
D9n     20         19 Dtvs
D10p    20         21 Dtvs
D10n    22         21 Dtvs
D11p    22         23 Dtvs
D11n    24         23 Dtvs
D12p    24         25 Dtvs
D12n    26         25 Dtvs
D13p    26         27 Dtvs
D13n    28         27 Dtvs
D14p    28         29 Dtvs
D14n    30         29 Dtvs
D15p    30         31 Dtvs
D15n    32         31 Dtvs
D16p    32         33 Dtvs
D16n    2          33 Dtvs
Rleak   1          2  19.0meg
.MODEL  Dtvs       D  (IS=1.0e-5 RS=0.000173 N=1.5 IBV=10m BV=12.63 CJO=35000p)
.ENDS

.SUBCKT AK10-240C  1  2
*       TERMINALS: T1 T2
D1p     1          3  Dtvs
D1n     4          3  Dtvs
D2p     4          5  Dtvs
D2n     6          5  Dtvs
D3p     6          7  Dtvs
D3n     8          7  Dtvs
D4p     8          9  Dtvs
D4n     10         9  Dtvs
D5p     10         11 Dtvs
D5n     12         11 Dtvs
D6p     12         13 Dtvs
D6n     14         13 Dtvs
D7p     14         15 Dtvs
D7n     16         15 Dtvs
D8p     16         17 Dtvs
D8n     18         17 Dtvs
D9p     18         19 Dtvs
D9n     20         19 Dtvs
D10p    20         21 Dtvs
D10n    22         21 Dtvs
D11p    22         23 Dtvs
D11n    24         23 Dtvs
D12p    24         25 Dtvs
D12n    26         25 Dtvs
D13p    26         27 Dtvs
D13n    28         27 Dtvs
D14p    28         29 Dtvs
D14n    30         29 Dtvs
D15p    30         31 Dtvs
D15n    32         31 Dtvs
D16p    32         33 Dtvs
D16n    34         33 Dtvs
D17p    34         35 Dtvs
D17n    36         35 Dtvs
D18p    36         37 Dtvs
D18n    2          37 Dtvs
Rleak   1          2  24.0meg
.MODEL  Dtvs       D  (IS=1.0e-5 RS=0.000133 N=1.5 IBV=10m BV=14.03 CJO=35000p)
.ENDS

.SUBCKT AK10-380C  1  2
*       TERMINALS: T1 T2
D1p     1          3  Dtvs
D1n     4          3  Dtvs
D2p     4          5  Dtvs
D2n     6          5  Dtvs
D3p     6          7  Dtvs
D3n     8          7  Dtvs
D4p     8          9  Dtvs
D4n     10         9  Dtvs
D5p     10         11 Dtvs
D5n     12         11 Dtvs
D6p     12         13 Dtvs
D6n     14         13 Dtvs
D7p     14         15 Dtvs
D7n     16         15 Dtvs
D8p     16         17 Dtvs
D8n     18         17 Dtvs
D9p     18         19 Dtvs
D9n     20         19 Dtvs
D10p    20         21 Dtvs
D10n    22         21 Dtvs
D11p    22         23 Dtvs
D11n    24         23 Dtvs
D12p    24         25 Dtvs
D12n    26         25 Dtvs
D13p    26         27 Dtvs
D13n    28         27 Dtvs
D14p    28         29 Dtvs
D14n    30         29 Dtvs
D15p    30         31 Dtvs
D15n    32         31 Dtvs
D16p    32         33 Dtvs
D16n    34         33 Dtvs
D17p    34         35 Dtvs
D17n    36         35 Dtvs
D18p    36         37 Dtvs
D18n    2          37 Dtvs
Rleak   1          2  38.0meg
.MODEL  Dtvs       D  (IS=1.0e-5 RS=0.000147 N=1.5 IBV=10m BV=22.50 CJO=35000p)
.ENDS

.SUBCKT AK10-430C  1  2
*       TERMINALS: T1 T2
D1p     1          3  Dtvs
D1n     4          3  Dtvs
D2p     4          5  Dtvs
D2n     6          5  Dtvs
D3p     6          7  Dtvs
D3n     8          7  Dtvs
D4p     8          9  Dtvs
D4n     10         9  Dtvs
D5p     10         11 Dtvs
D5n     12         11 Dtvs
D6p     12         13 Dtvs
D6n     14         13 Dtvs
D7p     14         15 Dtvs
D7n     16         15 Dtvs
D8p     16         17 Dtvs
D8n     18         17 Dtvs
D9p     18         19 Dtvs
D9n     20         19 Dtvs
D10p    20         21 Dtvs
D10n    22         21 Dtvs
D11p    22         23 Dtvs
D11n    24         23 Dtvs
D12p    24         25 Dtvs
D12n    26         25 Dtvs
D13p    26         27 Dtvs
D13n    28         27 Dtvs
D14p    28         29 Dtvs
D14n    30         29 Dtvs
D15p    30         31 Dtvs
D15n    32         31 Dtvs
D16p    32         33 Dtvs
D16n    34         33 Dtvs
D17p    34         35 Dtvs
D17n    36         35 Dtvs
D18p    36         37 Dtvs
D18n    38         37 Dtvs
D19p    38         39 Dtvs
D19n    40         39 Dtvs
D20p    40         41 Dtvs
D20n    2          41 Dtvs
Rleak   1          2  43.0meg
.MODEL  Dtvs       D  (IS=1.0e-5 RS=0.000274 N=1.5 IBV=10m BV=22.22 CJO=35000p)
.ENDS

.SUBCKT AK10-530C  1  2
*       TERMINALS: T1 T2
D1p     1          3  Dtvs
D1n     4          3  Dtvs
D2p     4          5  Dtvs
D2n     6          5  Dtvs
D3p     6          7  Dtvs
D3n     8          7  Dtvs
D4p     8          9  Dtvs
D4n     10         9  Dtvs
D5p     10         11 Dtvs
D5n     12         11 Dtvs
D6p     12         13 Dtvs
D6n     14         13 Dtvs
D7p     14         15 Dtvs
D7n     16         15 Dtvs
D8p     16         17 Dtvs
D8n     18         17 Dtvs
D9p     18         19 Dtvs
D9n     20         19 Dtvs
D10p    20         21 Dtvs
D10n    22         21 Dtvs
D11p    22         23 Dtvs
D11n    24         23 Dtvs
D12p    24         25 Dtvs
D12n    26         25 Dtvs
D13p    26         27 Dtvs
D13n    28         27 Dtvs
D14p    28         29 Dtvs
D14n    30         29 Dtvs
D15p    30         31 Dtvs
D15n    32         31 Dtvs
D16p    32         33 Dtvs
D16n    34         33 Dtvs
D17p    34         35 Dtvs
D17n    36         35 Dtvs
D18p    36         37 Dtvs
D18n    38         37 Dtvs
D19p    38         39 Dtvs
D19n    40         39 Dtvs
D20p    40         41 Dtvs
D20n    42         41 Dtvs
D21p    42         43 Dtvs
D21n    44         43 Dtvs
D22p    44         45 Dtvs
D22n    46         45 Dtvs
D23p    46         47 Dtvs
D23n    48         47 Dtvs
D24p    48         49 Dtvs
D24n    2           49 Dtvs
Rleak   1          2  53.0meg
.MODEL  Dtvs       D  (IS=1.0e-5 RS=0.000274 N=1.5 IBV=10m BV=23.54 CJO=35000p)
.ENDS
