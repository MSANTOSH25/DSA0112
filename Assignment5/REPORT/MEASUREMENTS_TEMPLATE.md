# Record your own experimental measurements
Record CPU, OS, `g++ --version`, compile flags and repeated-run median. Do not invent or copy timings.

| N bills | switch ms | virtual ms | variant ms | totals equal |
|---:|---:|---:|---:|---|
| 1,000 | | | | |
| 1,000,000 | | | | |
| 10,000,000 | | | | |

| Invalid rate | invalid count | return-code ms | exception ms |
|---:|---:|---:|---:|
| 0% | 0 | | |
| 1% | 10,000 | | |
| 50% | 500,000 | | |

The benchmark provided measures switch, virtual and variant totals. Add and time a separate return-code versus exception validation loop for B3.3.
