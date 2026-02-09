#  SpecTclGUI save file created Thu Jan 22 13:16:54 EST 2026
#  SpecTclGui Version: 1.0
#      Author: Ron Fox (fox@nscl.msu.edu)

#Tree params:


# Pseudo parameter definitions


# Tree variable definitions:

treevariable -set srs.chargeMethod 9 {charge calc. method}
treevariable -set srs.debugMode 1 debug
treevariable -set srs.posMethod 9 {position calc. method}
treevariable -set srs.timeMethod 9 {time calc. method}

# Gate definitions in reverse dependency order
 

# Spectrum Definitions

catch {spectrum -delete dc1.adc_s}
spectrum dc1.adc_s s {srs.fec2.adc.000 srs.fec2.adc.001 srs.fec2.adc.002 srs.fec2.adc.003 srs.fec2.adc.004 srs.fec2.adc.005 srs.fec2.adc.006 srs.fec2.adc.007 srs.fec2.adc.008 srs.fec2.adc.009 srs.fec2.adc.010 srs.fec2.adc.011 srs.fec2.adc.012 srs.fec2.adc.013 srs.fec2.adc.014 srs.fec2.adc.015 srs.fec2.adc.016 srs.fec2.adc.017 srs.fec2.adc.018 srs.fec2.adc.019 srs.fec2.adc.020 srs.fec2.adc.021 srs.fec2.adc.022 srs.fec2.adc.023 srs.fec2.adc.024 srs.fec2.adc.025 srs.fec2.adc.026 srs.fec2.adc.027 srs.fec2.adc.028 srs.fec2.adc.029 srs.fec2.adc.030 srs.fec2.adc.031 srs.fec2.adc.032 srs.fec2.adc.033 srs.fec2.adc.034 srs.fec2.adc.035 srs.fec2.adc.036 srs.fec2.adc.037 srs.fec2.adc.038 srs.fec2.adc.039 srs.fec2.adc.040 srs.fec2.adc.041 srs.fec2.adc.042 srs.fec2.adc.043 srs.fec2.adc.044 srs.fec2.adc.045 srs.fec2.adc.046 srs.fec2.adc.047 srs.fec2.adc.048 srs.fec2.adc.049 srs.fec2.adc.050 srs.fec2.adc.051 srs.fec2.adc.052 srs.fec2.adc.053 srs.fec2.adc.054 srs.fec2.adc.055 srs.fec2.adc.056 srs.fec2.adc.057 srs.fec2.adc.058 srs.fec2.adc.059 srs.fec2.adc.060 srs.fec2.adc.061 srs.fec2.adc.062 srs.fec2.adc.063 srs.fec2.adc.064 srs.fec2.adc.065 srs.fec2.adc.066 srs.fec2.adc.067 srs.fec2.adc.068 srs.fec2.adc.069 srs.fec2.adc.070 srs.fec2.adc.071 srs.fec2.adc.072 srs.fec2.adc.073 srs.fec2.adc.074 srs.fec2.adc.075 srs.fec2.adc.076 srs.fec2.adc.077 srs.fec2.adc.078 srs.fec2.adc.079 srs.fec2.adc.080 srs.fec2.adc.081 srs.fec2.adc.082 srs.fec2.adc.083 srs.fec2.adc.084 srs.fec2.adc.085 srs.fec2.adc.086 srs.fec2.adc.087 srs.fec2.adc.088 srs.fec2.adc.089 srs.fec2.adc.090 srs.fec2.adc.091 srs.fec2.adc.092 srs.fec2.adc.093 srs.fec2.adc.094 srs.fec2.adc.095 srs.fec2.adc.096 srs.fec2.adc.097 srs.fec2.adc.098 srs.fec2.adc.099 srs.fec2.adc.100 srs.fec2.adc.101 srs.fec2.adc.102 srs.fec2.adc.103 srs.fec2.adc.104 srs.fec2.adc.105 srs.fec2.adc.106 srs.fec2.adc.107 srs.fec2.adc.108 srs.fec2.adc.109 srs.fec2.adc.110 srs.fec2.adc.111 srs.fec2.adc.112 srs.fec2.adc.113 srs.fec2.adc.114 srs.fec2.adc.115 srs.fec2.adc.116 srs.fec2.adc.117 srs.fec2.adc.118 srs.fec2.adc.119 srs.fec2.adc.120 srs.fec2.adc.121 srs.fec2.adc.122 srs.fec2.adc.123 srs.fec2.adc.124 srs.fec2.adc.125 srs.fec2.adc.126 srs.fec2.adc.127 srs.fec2.adc.128 srs.fec2.adc.129 srs.fec2.adc.130 srs.fec2.adc.131 srs.fec2.adc.132 srs.fec2.adc.133 srs.fec2.adc.134 srs.fec2.adc.135 srs.fec2.adc.136 srs.fec2.adc.137 srs.fec2.adc.138 srs.fec2.adc.139 srs.fec2.adc.140 srs.fec2.adc.141 srs.fec2.adc.142 srs.fec2.adc.143 srs.fec2.adc.144 srs.fec2.adc.145 srs.fec2.adc.146 srs.fec2.adc.147 srs.fec2.adc.148 srs.fec2.adc.149 srs.fec2.adc.150 srs.fec2.adc.151 srs.fec2.adc.152 srs.fec2.adc.153 srs.fec2.adc.154 srs.fec2.adc.155 srs.fec2.adc.156 srs.fec2.adc.157 srs.fec2.adc.158 srs.fec2.adc.159 srs.fec2.adc.160 srs.fec2.adc.161 srs.fec2.adc.162 srs.fec2.adc.163 srs.fec2.adc.164 srs.fec2.adc.165 srs.fec2.adc.166 srs.fec2.adc.167 srs.fec2.adc.168 srs.fec2.adc.169 srs.fec2.adc.170 srs.fec2.adc.171 srs.fec2.adc.172 srs.fec2.adc.173 srs.fec2.adc.174 srs.fec2.adc.175 srs.fec2.adc.176 srs.fec2.adc.177 srs.fec2.adc.178 srs.fec2.adc.179 srs.fec2.adc.180 srs.fec2.adc.181 srs.fec2.adc.182 srs.fec2.adc.183 srs.fec2.adc.184 srs.fec2.adc.185 srs.fec2.adc.186 srs.fec2.adc.187 srs.fec2.adc.188 srs.fec2.adc.189 srs.fec2.adc.190 srs.fec2.adc.191 srs.fec2.adc.192 srs.fec2.adc.193 srs.fec2.adc.194 srs.fec2.adc.195 srs.fec2.adc.196 srs.fec2.adc.197 srs.fec2.adc.198 srs.fec2.adc.199 srs.fec2.adc.200 srs.fec2.adc.201 srs.fec2.adc.202 srs.fec2.adc.203 srs.fec2.adc.204 srs.fec2.adc.205 srs.fec2.adc.206 srs.fec2.adc.207 srs.fec2.adc.208 srs.fec2.adc.209 srs.fec2.adc.210 srs.fec2.adc.211 srs.fec2.adc.212 srs.fec2.adc.213 srs.fec2.adc.214 srs.fec2.adc.215 srs.fec2.adc.216 srs.fec2.adc.217 srs.fec2.adc.218 srs.fec2.adc.219 srs.fec2.adc.220 srs.fec2.adc.221 srs.fec2.adc.222 srs.fec2.adc.223 srs.fec2.adc.224 srs.fec2.adc.225 srs.fec2.adc.226 srs.fec2.adc.227 srs.fec2.adc.228 srs.fec2.adc.229 srs.fec2.adc.230 srs.fec2.adc.231 srs.fec2.adc.232 srs.fec2.adc.233 srs.fec2.adc.234 srs.fec2.adc.235 srs.fec2.adc.236 srs.fec2.adc.237 srs.fec2.adc.238 srs.fec2.adc.239 srs.fec2.adc.240 srs.fec2.adc.241 srs.fec2.adc.242 srs.fec2.adc.243 srs.fec2.adc.244 srs.fec2.adc.245 srs.fec2.adc.246 srs.fec2.adc.247 srs.fec2.adc.248 srs.fec2.adc.249 srs.fec2.adc.250 srs.fec2.adc.251 srs.fec2.adc.252 srs.fec2.adc.253 srs.fec2.adc.254 srs.fec2.adc.255} {{0.000000 1030.000000 1030}} long
catch {spectrum -delete dc1AdcSum}
spectrum dc1AdcSum s {srs.fec2.adc.000 srs.fec2.adc.001 srs.fec2.adc.002 srs.fec2.adc.003 srs.fec2.adc.004 srs.fec2.adc.005 srs.fec2.adc.006 srs.fec2.adc.007 srs.fec2.adc.008 srs.fec2.adc.009 srs.fec2.adc.010 srs.fec2.adc.011 srs.fec2.adc.012 srs.fec2.adc.013 srs.fec2.adc.014 srs.fec2.adc.015 srs.fec2.adc.016 srs.fec2.adc.017 srs.fec2.adc.018 srs.fec2.adc.019 srs.fec2.adc.020 srs.fec2.adc.021 srs.fec2.adc.022 srs.fec2.adc.023 srs.fec2.adc.024 srs.fec2.adc.025 srs.fec2.adc.026 srs.fec2.adc.027 srs.fec2.adc.028 srs.fec2.adc.029 srs.fec2.adc.030 srs.fec2.adc.031 srs.fec2.adc.032 srs.fec2.adc.033 srs.fec2.adc.034 srs.fec2.adc.035 srs.fec2.adc.036 srs.fec2.adc.037 srs.fec2.adc.038 srs.fec2.adc.039 srs.fec2.adc.040 srs.fec2.adc.041 srs.fec2.adc.042 srs.fec2.adc.043 srs.fec2.adc.044 srs.fec2.adc.045 srs.fec2.adc.046 srs.fec2.adc.047 srs.fec2.adc.048 srs.fec2.adc.049 srs.fec2.adc.050 srs.fec2.adc.051 srs.fec2.adc.052 srs.fec2.adc.053 srs.fec2.adc.054 srs.fec2.adc.055 srs.fec2.adc.056 srs.fec2.adc.057 srs.fec2.adc.058 srs.fec2.adc.059 srs.fec2.adc.060 srs.fec2.adc.061 srs.fec2.adc.062 srs.fec2.adc.063 srs.fec2.adc.064 srs.fec2.adc.065 srs.fec2.adc.066 srs.fec2.adc.067 srs.fec2.adc.068 srs.fec2.adc.069 srs.fec2.adc.070 srs.fec2.adc.071 srs.fec2.adc.072 srs.fec2.adc.073 srs.fec2.adc.074 srs.fec2.adc.075 srs.fec2.adc.076 srs.fec2.adc.077 srs.fec2.adc.078 srs.fec2.adc.079 srs.fec2.adc.080 srs.fec2.adc.081 srs.fec2.adc.082 srs.fec2.adc.083 srs.fec2.adc.084 srs.fec2.adc.085 srs.fec2.adc.086 srs.fec2.adc.087 srs.fec2.adc.088 srs.fec2.adc.089 srs.fec2.adc.090 srs.fec2.adc.091 srs.fec2.adc.092 srs.fec2.adc.093 srs.fec2.adc.094 srs.fec2.adc.095 srs.fec2.adc.096 srs.fec2.adc.097 srs.fec2.adc.098 srs.fec2.adc.099 srs.fec2.adc.100 srs.fec2.adc.101 srs.fec2.adc.102 srs.fec2.adc.103 srs.fec2.adc.104 srs.fec2.adc.105 srs.fec2.adc.106 srs.fec2.adc.107 srs.fec2.adc.108 srs.fec2.adc.109 srs.fec2.adc.110 srs.fec2.adc.111 srs.fec2.adc.112 srs.fec2.adc.113 srs.fec2.adc.114 srs.fec2.adc.115 srs.fec2.adc.116 srs.fec2.adc.117 srs.fec2.adc.118 srs.fec2.adc.119 srs.fec2.adc.120 srs.fec2.adc.121 srs.fec2.adc.122 srs.fec2.adc.123 srs.fec2.adc.124 srs.fec2.adc.125 srs.fec2.adc.126 srs.fec2.adc.127 srs.fec2.adc.128 srs.fec2.adc.129 srs.fec2.adc.130 srs.fec2.adc.131 srs.fec2.adc.132 srs.fec2.adc.133 srs.fec2.adc.134 srs.fec2.adc.135 srs.fec2.adc.136 srs.fec2.adc.137 srs.fec2.adc.138 srs.fec2.adc.139 srs.fec2.adc.140 srs.fec2.adc.141 srs.fec2.adc.142 srs.fec2.adc.143 srs.fec2.adc.144 srs.fec2.adc.145 srs.fec2.adc.146 srs.fec2.adc.147 srs.fec2.adc.148 srs.fec2.adc.149 srs.fec2.adc.150 srs.fec2.adc.151 srs.fec2.adc.152 srs.fec2.adc.153 srs.fec2.adc.154 srs.fec2.adc.155 srs.fec2.adc.156 srs.fec2.adc.157 srs.fec2.adc.158 srs.fec2.adc.159 srs.fec2.adc.160 srs.fec2.adc.161 srs.fec2.adc.162 srs.fec2.adc.163 srs.fec2.adc.164 srs.fec2.adc.165 srs.fec2.adc.166 srs.fec2.adc.167 srs.fec2.adc.168 srs.fec2.adc.169 srs.fec2.adc.170 srs.fec2.adc.171 srs.fec2.adc.172 srs.fec2.adc.173 srs.fec2.adc.174 srs.fec2.adc.175 srs.fec2.adc.176 srs.fec2.adc.177 srs.fec2.adc.178 srs.fec2.adc.179 srs.fec2.adc.180 srs.fec2.adc.181 srs.fec2.adc.182 srs.fec2.adc.183 srs.fec2.adc.184 srs.fec2.adc.185 srs.fec2.adc.186 srs.fec2.adc.187 srs.fec2.adc.188 srs.fec2.adc.189 srs.fec2.adc.190 srs.fec2.adc.191 srs.fec2.adc.192 srs.fec2.adc.193 srs.fec2.adc.194 srs.fec2.adc.195 srs.fec2.adc.196 srs.fec2.adc.197 srs.fec2.adc.198 srs.fec2.adc.199 srs.fec2.adc.200 srs.fec2.adc.201 srs.fec2.adc.202 srs.fec2.adc.203 srs.fec2.adc.204 srs.fec2.adc.205 srs.fec2.adc.206 srs.fec2.adc.207 srs.fec2.adc.208 srs.fec2.adc.209 srs.fec2.adc.210 srs.fec2.adc.211 srs.fec2.adc.212 srs.fec2.adc.213 srs.fec2.adc.214 srs.fec2.adc.215 srs.fec2.adc.216 srs.fec2.adc.217 srs.fec2.adc.218 srs.fec2.adc.219 srs.fec2.adc.220 srs.fec2.adc.221 srs.fec2.adc.222 srs.fec2.adc.223 srs.fec2.adc.224 srs.fec2.adc.225 srs.fec2.adc.226 srs.fec2.adc.227 srs.fec2.adc.228 srs.fec2.adc.229 srs.fec2.adc.230 srs.fec2.adc.231 srs.fec2.adc.232 srs.fec2.adc.233 srs.fec2.adc.234 srs.fec2.adc.235 srs.fec2.adc.236 srs.fec2.adc.237 srs.fec2.adc.238 srs.fec2.adc.239 srs.fec2.adc.240 srs.fec2.adc.241 srs.fec2.adc.242 srs.fec2.adc.243 srs.fec2.adc.244 srs.fec2.adc.245 srs.fec2.adc.246 srs.fec2.adc.247 srs.fec2.adc.248 srs.fec2.adc.249 srs.fec2.adc.250 srs.fec2.adc.251 srs.fec2.adc.252 srs.fec2.adc.253 srs.fec2.adc.254 srs.fec2.adc.255 srs.fec2.adc.256 srs.fec2.adc.257 srs.fec2.adc.258 srs.fec2.adc.259 srs.fec2.adc.260 srs.fec2.adc.261 srs.fec2.adc.262 srs.fec2.adc.263 srs.fec2.adc.264 srs.fec2.adc.265 srs.fec2.adc.266 srs.fec2.adc.267 srs.fec2.adc.268 srs.fec2.adc.269 srs.fec2.adc.270 srs.fec2.adc.271 srs.fec2.adc.272 srs.fec2.adc.273 srs.fec2.adc.274 srs.fec2.adc.275 srs.fec2.adc.276 srs.fec2.adc.277 srs.fec2.adc.278 srs.fec2.adc.279 srs.fec2.adc.280 srs.fec2.adc.281 srs.fec2.adc.282 srs.fec2.adc.283 srs.fec2.adc.284 srs.fec2.adc.285 srs.fec2.adc.286 srs.fec2.adc.287 srs.fec2.adc.288 srs.fec2.adc.289 srs.fec2.adc.290 srs.fec2.adc.291 srs.fec2.adc.292 srs.fec2.adc.293 srs.fec2.adc.294 srs.fec2.adc.295 srs.fec2.adc.296 srs.fec2.adc.297 srs.fec2.adc.298 srs.fec2.adc.299 srs.fec2.adc.300 srs.fec2.adc.301 srs.fec2.adc.302 srs.fec2.adc.303 srs.fec2.adc.304 srs.fec2.adc.305 srs.fec2.adc.306 srs.fec2.adc.307 srs.fec2.adc.308 srs.fec2.adc.309 srs.fec2.adc.310 srs.fec2.adc.311 srs.fec2.adc.312 srs.fec2.adc.313 srs.fec2.adc.314 srs.fec2.adc.315 srs.fec2.adc.316 srs.fec2.adc.317 srs.fec2.adc.318 srs.fec2.adc.319 srs.fec2.adc.320 srs.fec2.adc.321 srs.fec2.adc.322 srs.fec2.adc.323 srs.fec2.adc.324 srs.fec2.adc.325 srs.fec2.adc.326 srs.fec2.adc.327 srs.fec2.adc.328 srs.fec2.adc.329 srs.fec2.adc.330 srs.fec2.adc.331 srs.fec2.adc.332 srs.fec2.adc.333 srs.fec2.adc.334 srs.fec2.adc.335 srs.fec2.adc.336 srs.fec2.adc.337 srs.fec2.adc.338 srs.fec2.adc.339 srs.fec2.adc.340 srs.fec2.adc.341 srs.fec2.adc.342 srs.fec2.adc.343 srs.fec2.adc.344 srs.fec2.adc.345 srs.fec2.adc.346 srs.fec2.adc.347 srs.fec2.adc.348 srs.fec2.adc.349 srs.fec2.adc.350 srs.fec2.adc.351 srs.fec2.adc.352 srs.fec2.adc.353 srs.fec2.adc.354 srs.fec2.adc.355 srs.fec2.adc.356 srs.fec2.adc.357 srs.fec2.adc.358 srs.fec2.adc.359 srs.fec2.adc.360 srs.fec2.adc.361 srs.fec2.adc.362 srs.fec2.adc.363 srs.fec2.adc.364 srs.fec2.adc.365 srs.fec2.adc.366 srs.fec2.adc.367 srs.fec2.adc.368 srs.fec2.adc.369 srs.fec2.adc.370 srs.fec2.adc.371 srs.fec2.adc.372 srs.fec2.adc.373 srs.fec2.adc.374 srs.fec2.adc.375 srs.fec2.adc.376 srs.fec2.adc.377 srs.fec2.adc.378 srs.fec2.adc.379 srs.fec2.adc.380 srs.fec2.adc.381 srs.fec2.adc.382 srs.fec2.adc.383 srs.fec2.adc.384 srs.fec2.adc.385 srs.fec2.adc.386 srs.fec2.adc.387 srs.fec2.adc.388 srs.fec2.adc.389 srs.fec2.adc.390 srs.fec2.adc.391 srs.fec2.adc.392 srs.fec2.adc.393 srs.fec2.adc.394 srs.fec2.adc.395 srs.fec2.adc.396 srs.fec2.adc.397 srs.fec2.adc.398 srs.fec2.adc.399 srs.fec2.adc.400 srs.fec2.adc.401 srs.fec2.adc.402 srs.fec2.adc.403 srs.fec2.adc.404 srs.fec2.adc.405 srs.fec2.adc.406 srs.fec2.adc.407 srs.fec2.adc.408 srs.fec2.adc.409 srs.fec2.adc.410 srs.fec2.adc.411 srs.fec2.adc.412 srs.fec2.adc.413 srs.fec2.adc.414 srs.fec2.adc.415 srs.fec2.adc.416 srs.fec2.adc.417 srs.fec2.adc.418 srs.fec2.adc.419 srs.fec2.adc.420 srs.fec2.adc.421 srs.fec2.adc.422 srs.fec2.adc.423 srs.fec2.adc.424 srs.fec2.adc.425 srs.fec2.adc.426 srs.fec2.adc.427 srs.fec2.adc.428 srs.fec2.adc.429 srs.fec2.adc.430 srs.fec2.adc.431 srs.fec2.adc.432 srs.fec2.adc.433 srs.fec2.adc.434 srs.fec2.adc.435 srs.fec2.adc.436 srs.fec2.adc.437 srs.fec2.adc.438 srs.fec2.adc.439 srs.fec2.adc.440 srs.fec2.adc.441 srs.fec2.adc.442 srs.fec2.adc.443 srs.fec2.adc.444 srs.fec2.adc.445 srs.fec2.adc.446 srs.fec2.adc.447 srs.fec2.adc.448 srs.fec2.adc.449 srs.fec2.adc.450 srs.fec2.adc.451 srs.fec2.adc.452 srs.fec2.adc.453 srs.fec2.adc.454 srs.fec2.adc.455 srs.fec2.adc.456 srs.fec2.adc.457 srs.fec2.adc.458 srs.fec2.adc.459 srs.fec2.adc.460 srs.fec2.adc.461 srs.fec2.adc.462 srs.fec2.adc.463 srs.fec2.adc.464 srs.fec2.adc.465 srs.fec2.adc.466 srs.fec2.adc.467 srs.fec2.adc.468 srs.fec2.adc.469 srs.fec2.adc.470 srs.fec2.adc.471 srs.fec2.adc.472 srs.fec2.adc.473 srs.fec2.adc.474 srs.fec2.adc.475 srs.fec2.adc.476 srs.fec2.adc.477 srs.fec2.adc.478 srs.fec2.adc.479} {{0.000000 1023.000000 1024}} long
catch {spectrum -delete srs.fec2.chargeCluster}
spectrum srs.fec2.chargeCluster 1 srs.fec2.chargeCluster {{0.000000 2000.000000 200}} long
catch {spectrum -delete srs.fec2.chargeCluster_avg_g}
spectrum srs.fec2.chargeCluster_avg_g 1 srs.fec2.chargeCluster {{0.000000 500.000000 500}} long
catch {spectrum -delete srs.fec2.chargeCluster_bg}
spectrum srs.fec2.chargeCluster_bg 1 srs.fec2.chargeCluster {{0.000000 2000.000000 200}} long
catch {spectrum -delete srs.fec2.chargeCluster_g}
spectrum srs.fec2.chargeCluster_g 1 srs.fec2.chargeCluster {{0.000000 2000.000000 400}} long
catch {spectrum -delete srs.fec2.chargeCluster_gAll}
spectrum srs.fec2.chargeCluster_gAll 1 srs.fec2.chargeCluster {{0.000000 2000.000000 400}} long
catch {spectrum -delete srs.fec2.chargeCluster_hnbHits}
spectrum srs.fec2.chargeCluster_hnbHits 1 srs.fec2.chargeCluster {{0.000000 2000.000000 200}} long
catch {spectrum -delete srs.fec2.chargeCluster_tc}
spectrum srs.fec2.chargeCluster_tc 1 srs.fec2.chargeCluster {{0.000000 2000.000000 200}} long
catch {spectrum -delete srs.fec2.posCluster}
spectrum srs.fec2.posCluster 1 srs.fec2.posCluster {{1.000000 600.000000 600}} long
catch {spectrum -delete srs.fec2.posCluster_g}
spectrum srs.fec2.posCluster_g 1 srs.fec2.posCluster {{1.000000 600.000000 600}} long
catch {spectrum -delete srs.fec2.poscharge}
spectrum srs.fec2.poscharge 2 {srs.fec2.posCluster srs.fec2.chargeCluster} {{1.000000 500.000000 500} {1.000000 2000.000000 400}} long
catch {spectrum -delete srs.fec2.poscharge_g}
spectrum srs.fec2.poscharge_g 2 {srs.fec2.posCluster srs.fec2.chargeCluster} {{0.000000 500.000000 501} {1.000000 2000.000000 400}} long
catch {spectrum -delete srs.fec2.poscharge_gAll}
spectrum srs.fec2.poscharge_gAll 2 {srs.fec2.posCluster srs.fec2.chargeCluster} {{1.000000 500.000000 500} {1.000000 2000.000000 400}} long
catch {spectrum -delete srs.fec2.postimeCluster}
spectrum srs.fec2.postimeCluster 2 {srs.fec2.posCluster srs.fec2.timeCluster} {{1.000000 600.000000 600} {-50.000000 100.000000 150}} long
catch {spectrum -delete srs.fec2.postimeCluster_g}
spectrum srs.fec2.postimeCluster_g 2 {srs.fec2.posCluster srs.fec2.timeCluster} {{1.000000 600.000000 600} {-50.000000 100.000000 150}} long
catch {spectrum -delete srs.fec2.timeCluster}
spectrum srs.fec2.timeCluster 1 srs.fec2.timeCluster {{-50.000000 100.000000 150}} long
catch {spectrum -delete srs.fec2.timeCluster_g}
spectrum srs.fec2.timeCluster_g 1 srs.fec2.timeCluster {{-50.000000 100.000000 150}} long
catch {spectrum -delete srs.fec2.timecharge}
spectrum srs.fec2.timecharge 2 {srs.fec2.timeCluster srs.fec2.chargeCluster} {{-50.000000 100.000000 150} {1.000000 2000.000000 400}} long
catch {spectrum -delete srs.fec2.timecharge_gAll}
spectrum srs.fec2.timecharge_gAll 2 {srs.fec2.timeCluster srs.fec2.chargeCluster} {{-50.000000 100.000000 150} {1.000000 2000.000000 400}} long
catch {spectrum -delete srs.nHits}
spectrum srs.nHits 1 srs.nHits {{0.000000 50.000000 50}} long

# Gate Applications: 


#  filter definitions: ALL FILTERS ARE DISABLED!!!!!!!


#  - Parameter tab layout: 

set parameter(select) 1
set parameter(Array)  false

#-- Variable tab layout

set variable(Name1) srs.chargeMethod
set variable(Value1) 9
set variable(Unit1) {charge calc. method}
set variable(Name2) srs.posMethod
set variable(Value2) 9
set variable(Unit2) {position calc. method}
set variable(Name3) srs.timeMethod
set variable(Value3) 9
set variable(Unit3) {time calc. method}
set variable(Name4) srs.debugMode
set variable(Value4) 1
set variable(Unit4) debug
set variable(select) 4
set variable(Array)  0
