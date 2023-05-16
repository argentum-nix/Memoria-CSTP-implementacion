
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% CONJUNTOS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


set N;	
										#Conjunto nodos
set Victimas;												#Conjunto víctimas
set Victimas2 ordered;
set M;												#Conjunto de vehículos
set E;												#Conjunto de tipo de vehículos
set H within N;										#Conjunto hospitales
set Hf within H default {};							#Conjunto Centros de Atencion Temporal
param VMAX default 0;								#Parámetro de vueltas por default
set V := 1..VMAX;									#Conjunto de vueltas
set G;												#Conjunto de gravedad de víctimas
set PPP; 
set VT default {}; #Victimas atendidas en la corrida
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PARÁMETROS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


param PA{Victimas};										#UbiVictimasción de las víctimas
param GR{Victimas,G} default 0;							#Gravedad de la víctima con default 0			

param ME{M,E} default 0;							#IndiVictimas de que tipo es el vehículo m 
param O{M} within H;								#Punto de partida de los hospitales
param Q{E};											#Victimaspacidad de los vehículos
param Sx{(H union Victimas),(H union Victimas),E} default 0;	#Matriz distancias considerando distancia entre víctimas
param S{N,N,E};										#Matriz de distancias

param K{H,G};										#Victimaspacidad de los hospitales

param TA{Victimas};										#Tiempo en que aparece victima j
param TP{Victimas};										#Tiempo de preparación de las víctimas según gravedad
param TD{M};										#Tiempo de despliegue de operaciones
param TAT{E};										#Tiempo de aterrizaje
param TDS{E};										#Tiempo de despegue 

param PESO{G};										#Ponderación de las gravedades

param EPSILON default 0;							#Parámetro de transformación monetarios
param M2;											#Valor muy alto
param B:= 100000000;
param M3:=3;

param Peso2{Victimas} default 0;
param TEmax1:= 1140;
#param TEmax2:= 500;
param TEmax2:= 360;
param TE{Victimas} default 0;
param TE2{Victimas} default 0;
param TT;
param TV{Victimas} default 0;
param TCPU;

param NA{Victimas};
param t{Victimas};
param EDAD{Victimas};

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% VARIABLES DE DECISIÓN %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


var x{N,N,M,V} binary; 								#1 si la víctima j es sucedida por la víctima i en un vehículo m en la vuelta v, 0 en otro VTso.
var y{VT,H,M,V} binary;								#1 si el vehículo m traslada a la víctima j al hospital h en la vuelta v, 0 en otro VTso.
var z{H,M,V} binary;								#1 si vehículo m termina su vuelta v en hospital h, 0 en otro VTso.
var w{VT,M,V} binary;								#1 si la víctima j es atendida por el vehículo m en la vuelta v, 0 en otro VTso.
var u{M,V} binary;									#1 el vehículo m ejecuta una vuelta v, 0 en otro VTso.
var F{H} binary;									#1 si se activa centro de VTmpaña temporal h, 0 si no.

var C{VT} >=0;										#Tiempo que demora la ambulancia en atender a la víctima j.
var T{M,V} >= 0;									#Tiempo que se demora el vehículo m en realizar vuelta v.
var beta{VT} >= 0;									#Tiempo en que la víctima j llega al hospital.



#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FUNCIÓN OBJETIVO %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#minimize pruebaz: sum{j in VT} (Peso2[j]*beta[j])+ 100000*sum{h in H}F[h] +0.00001*sum{h in H, g in G} L[h,g] + 0.0000001*sum{j in VT} C[j];
#minimize pruebaz: sum{j in VT} Peso2[j]*(0.0000*beta[j]+1*C[j]) + 100000*sum{h in H}F[h];
#minimize FO1: sum{j in VT} Peso2[j]*(beta[j]) + 100000*sum{h in H}F[h];
minimize FO: sum{j in VT} Peso2[j]*(C[j])  + 100000*sum{h in H}F[h] + sum{j in VT} 0.000001*beta[j];



#minimize pruebaz: sum{j in VT} Peso2[j]*(beta[j]) + 100000*sum{h in H}F[h] + sum{j in VT} 0.00000001*C[j];

subject to


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% SECUENCIA DE VÍCTIMAS/VUELTAS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

# El punto de término de una vuelta debe ser igual al punto de inicio de la vuelta siguiente.
r100{m in M, v in V diff {VMAX}, h in H}: z[h,m,v] = sum{i in (VT union H)} x[h,i,m,(v+1)];

#Evita que existan recorridos innecesarios entre hospitales distintos.
r109{i in H, j in H, m in M, v in V: i <> j}: x[i,j,m,v] = 0;

#Todo vehículo parte de su origen.
r2{m in M}: sum{j in (VT union H)} x[O[m],j,m,1] = 1;

# La recolección de las víctimas debe ser sucesiva.
r3{a in VT, m in M, v in V}: ((sum{i in (VT union H): i <> a } x[i,a,m,v])-(sum{j in (VT union H):j<>a} x[a,j,m,v])) = 0;

#Toda vuelta de todo vehículo debe terminar en un hospital.
r4{m in M, v in V}: sum{h in H} z[h,m,v] = 1;

#Las vueltas deben ser sucesivas y secuenciales.
r5{m in M, v in V diff {VMAX}}: u[m,v] >= u[m,(v+1)];

#IndiVT las vueltas en que efectivamente se trasladan víctimas.
r6{j in VT, m in M, v in V}: u[m,v] >= w[j,m,v];
r7{m in M, v in V}: u[m,v] <= sum{j in VT} w[j,m,v];

#Toda víctima debe ser atendida por un único vehículo en una úniVT vuelta.
r8{j in VT}: sum{v in V, m in M} w[j,m,v] = 1;

#Evita que se recoja la misma víctima dentro de una vuelta.
r9{i in VT, m in M, v in V}: x[i,i,m,v] = 0;




#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% TIEMPOS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#VTlculo de recolección de las víctimas (para la primera víctima de la primera vuelta).
r12{j in VT, m in M, h in H}: C[j] >= sum{e in E} (ME[m,e]*(TD[m]+TAT[e] + Sx[O[m],j,e])) + (ME[m,2]*t[j]+TP[j]) - M2 * (1 - w[j,m,1]) - M2 * ( 1 - x[O[m],j,m,1]);

#VTlculo de recolección de las víctimas (cuando las víctimas pertenecen a una misma vuelta y las víctimas estan en el mismo nodo)
r13{i in VT, j in VT, m in M: PA[i] = PA[j]}: C[j] >= C[i] + ME[m,2]*(t[i] + t[j]) + (TP[j]) +  M2 * (sum{v in V} x[i,j,m,v] - 1);

#VTlculo de recolección de las víctimas (cuando las víctimas pertenecen a una misma vuelta y las víctimas estan en distinto nodo).
r14{i in VT, j in VT, m in M: PA[i] <> PA[j]}: C[j] >= C[i] + ME[m,2]*(t[i] + t[j])+ sum{e in E} (ME[m,e]*Sx[i,j,e]) + sum{e in E:NA[i] <> NA[j]} (ME[m,e]*(TDS[e]+TAT[e])) + (TP[j]) + M2 * (sum{v in V} x[i,j,m,v] - 1);

#VTlculo de recolección de las víctimas (cuando las víctimas NO pertenecen a una misma vuelta).
r15{j in VT, m in M, h in H, v in V diff {1}}: C[j] >= T[m,(v-1)] +  sum{e in E} (ME[m,e]*(TDS[e]+TAT[e]+Sx[h,j,e])) + (TP[j]) +  ME[m,2]*t[j] - M2 * (1 - w[j,m,v]) - M2 * ( 1 - z[h,m,(v-1)]);


#r135{i in VT, j in VT, m in M}: C[j] >= C[i] + sum{e in E: PA[i] <> PA[j]} (ME[m,e]*(TDS[e]+TAT[e]+Sx[i,j,e])) + sum{g in G} (GR[j,g]*TP[g]) + M2 * (sum{v in V} x[i,j,m,v] - 1);






#VTlculo los tiempos de recolección para la primera vuelta.
#r16{m in M}: T[m,1] = sum{j in VT, g in G} (GR[j,g]*TP[g]+2*ME[m,2]*t[j])*w[j,m,1] +
#sum{e in E, j in VT} (ME[m,e]*(TD[m]+TAT[e])*x[O[m],j,m,1]) + 
#sum{i in (VT union H), j in (VT union H), e in E}((Sx[i,j,e]*ME[m,e]) * x[i,j,m,1])+
#sum{j in VT, i in VT: NA[i] <> NA[j] } (ME[m,2]*(TDS[2] + TAT[2])*x[i,j,m,1]) + 
#sum{j in VT, h in H} (ME[m,2]*(TDS[2] + TAT[2])*x[j,h,m,1])
#;


r16{m in M}: T[m,1] =
sum{j in VT} ((TP[j]+2*ME[m,2]*t[j])*w[j,m,1])
+sum{e in E, j in VT} (ME[m,e]*(TD[m]+TAT[e])*x[O[m],j,m,1])  
+sum{i in (VT union H), j in (VT union H), e in E}((Sx[i,j,e]*ME[m,e]) * x[i,j,m,1])
+sum{j in VT, i in VT: NA[i] <> NA[j] } (ME[m,2]*(TDS[2] + TAT[2])*x[i,j,m,1]) 
+sum{j in VT, h in H} (ME[m,2]*(TDS[2] + TAT[2])*x[j,h,m,1])
;

#r16{m in M}: T[m,1] = sum{j in VT, g in G} (GR[j,g]*TP[g]+2*ME[m,2]*t[j])*w[j,m,1] +
#sum{e in E, j in VT} (ME[m,e]*(TD[m]+TAT[e])*x[O[m],j,m,1]) + 
#sum{i in (VT union H), j in (VT union H)}((Sx[i,j,1]*ME[m,1]) * x[i,j,m,1])+
#sum{i in H, j in VT}((Sx[i,j,2]*ME[m,2])* x[i,j,m,1])+
#sum{i in VT, j in H}((Sx[i,j,2]*ME[m,2])* x[i,j,m,1])+
#sum{i in VT, j in VT: NA[i] <> NA[j] }((Sx[i,j,2]*ME[m,2])* x[i,j,m,1])+
#sum{e in E, j in VT, i in VT: PA[i] <> PA[j] } (ME[m,e]*(TDS[e] + TAT[e])*x[i,j,m,1]) + 
#sum{e in E, j in VT, h in H} (ME[m,e]*(TDS[e] + TAT[e])*x[j,h,m,1])
;



#VTlculo de los tiempos de recolección para VTda vuelta.
r17{v in V diff {1}, m in M}: T[m,v] = T[m,(v-1)] 
+ sum{j in VT} ((TP[j]+2*ME[m,2]*t[j])*w[j,m,v])
+ sum{i in (VT union H),j in (VT union H), e in E} (Sx[i,j,e] * ME[m,e] * x[i,j,m,v])
+ sum{e in E, i in VT,h in H} (ME[m,e]*(TDS[e]+TAT[e])*x[h,i,m,v])
+ sum{e in E, j in VT, i in VT: NA[i] <> NA[j] } (ME[m,e]*(TDS[e] + TAT[e])*x[i,j,m,v]) 
+ sum{j in VT, h in H, e in E} ((TDS[e] + TAT[e]) * ME[m,e] * x[j,h,m,v]) 
; 



#El cálculo del tiempo sólo puede ser realizado si la víctima j está en movimiento.
r20{j in VT, m in M, v in V}: beta[j] >= T[m,v] - M2*(1-w[j,m,v]);


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% VTPACIDAD %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#VTpacidad de los hospitales fijos
r23{g in G, h in H diff{Hf}}: sum{m in M, j in VT, v in V} (y[j,h,m,v] * GR [j,g]) <= K[h,g];

#VTpacidad de los hospitales ficticios
r24{g in G, h in Hf}: sum{m in M, j in VT, v in V} (y[j,h,m,v] * GR [j,g]) <= K[h,g] * F[h];

#VTpacidad de los vehículos
r25{m in M, v in V}: sum{j in VT} w[j,m,v] <= sum{e in E} (Q[e] * ME[m,e]);

#r235{g in G, h in H}: sum{m in M, j in VT, v in V} (y[j,h,m,v] * GR [j,g]) <= (K[h,g])* F[h] ;


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% HOSPITALES FICTICIOS Y SOBREVTPACIDAD %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#Activación de hospitales ficticios
r26{g in G}: (sum{m in M, h in H, j in VT, v in V} (y[j,h,m,v] * GR[j,g])) - (sum{h in H} (K[h,g])) <= M2 * sum{h in Hf} F[h];

#Límites de la sobreVTpacidad de los hospitales
#r27{g in G, h in H diff {Hf}}: L[h,g] <= R[h,g];
#r28{g in G, h in Hf}: L[h,g] <= R[h,g] * F[h];
#r275{g in G, h in H}: L[h,g] <= R[h,g];

#Las víctimas pueden ir a los hospitales ficticios sólo si estos han sido activados.
r29{i in (VT union H), h in Hf, m in M, v in V}: x[i,h,m,v] <= F[h];
r30{i in (VT union H), h in Hf, m in M, v in V}: x[h,i,m,v] <= F[h];
#r295{i in VT, h in Hf, m in M, v in V}: x[h,i,m,v] + x[i,h,m,v] <= 2*F[h];




#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% RELACIÓN ENTRE VARIABLES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


r31{j in VT, m in M, v in V}: sum{i in (VT union H)} x[i,j,m,v] = sum{h in H} y[j,h,m,v];

r32{j in VT, m in M, v in V}: w[j,m,v] = sum{i in (VT union H)} x[i,j,m,v];

r33{h in H, m in M, v in V}: sum{j in VT} y[j,h,m,v] <= M3 * sum{e in E} (ME[m,e] * z[h,m,v]) ;

r34{h in H, m in M, v in V}: sum{i in (VT union H)} (x[i,h,m,v]) = z[h,m,v] ;

r37{m in M, v in V, i in VT, j in VT: j!= i}: x[i,j,m,v] + x[j,i,m,v] <= 1;

r38{m in M, v in V, i in VT, j in VT, k in VT: j!=i and j!=k and k!=i}: x[i,j,m,v] + x[j,k,m,v] + x[k,i,m,v] <= 2;
 

#r400: F[10000] = 1;
#r401: F[10001] = 1;
#r402: F[10002] = 1;
#r403: F[30000] = 1;