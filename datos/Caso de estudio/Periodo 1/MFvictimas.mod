
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% CONJUNTOS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


set N;	
										#Conjunto nodos
set Victimas;												#Conjunto v�ctimas
set Victimas2 ordered;
set M;												#Conjunto de veh�culos
set E;												#Conjunto de tipo de veh�culos
set H within N;										#Conjunto hospitales
set Hf within H default {};							#Conjunto Centros de Atencion Temporal
param VMAX default 0;								#Par�metro de vueltas por default
set V := 1..VMAX;									#Conjunto de vueltas
set G;												#Conjunto de gravedad de v�ctimas
set PPP; 
set VT default {}; #Victimas atendidas en la corrida
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PAR�METROS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


param PA{Victimas};										#UbiVictimasci�n de las v�ctimas
param GR{Victimas,G} default 0;							#Gravedad de la v�ctima con default 0			

param ME{M,E} default 0;							#IndiVictimas de que tipo es el veh�culo m 
param O{M} within H;								#Punto de partida de los hospitales
param Q{E};											#Victimaspacidad de los veh�culos
param Sx{(H union Victimas),(H union Victimas),E} default 0;	#Matriz distancias considerando distancia entre v�ctimas
param S{N,N,E};										#Matriz de distancias

param K{H,G};										#Victimaspacidad de los hospitales

param TA{Victimas};										#Tiempo en que aparece victima j
param TP{Victimas};										#Tiempo de preparaci�n de las v�ctimas seg�n gravedad
param TD{M};										#Tiempo de despliegue de operaciones
param TAT{E};										#Tiempo de aterrizaje
param TDS{E};										#Tiempo de despegue 

param PESO{G};										#Ponderaci�n de las gravedades

param EPSILON default 0;							#Par�metro de transformaci�n monetarios
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

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% VARIABLES DE DECISI�N %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


var x{N,N,M,V} binary; 								#1 si la v�ctima j es sucedida por la v�ctima i en un veh�culo m en la vuelta v, 0 en otro VTso.
var y{VT,H,M,V} binary;								#1 si el veh�culo m traslada a la v�ctima j al hospital h en la vuelta v, 0 en otro VTso.
var z{H,M,V} binary;								#1 si veh�culo m termina su vuelta v en hospital h, 0 en otro VTso.
var w{VT,M,V} binary;								#1 si la v�ctima j es atendida por el veh�culo m en la vuelta v, 0 en otro VTso.
var u{M,V} binary;									#1 el veh�culo m ejecuta una vuelta v, 0 en otro VTso.
var F{H} binary;									#1 si se activa centro de VTmpa�a temporal h, 0 si no.

var C{VT} >=0;										#Tiempo que demora la ambulancia en atender a la v�ctima j.
var T{M,V} >= 0;									#Tiempo que se demora el veh�culo m en realizar vuelta v.
var beta{VT} >= 0;									#Tiempo en que la v�ctima j llega al hospital.



#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FUNCI�N OBJETIVO %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#minimize pruebaz: sum{j in VT} (Peso2[j]*beta[j])+ 100000*sum{h in H}F[h] +0.00001*sum{h in H, g in G} L[h,g] + 0.0000001*sum{j in VT} C[j];
#minimize pruebaz: sum{j in VT} Peso2[j]*(0.0000*beta[j]+1*C[j]) + 100000*sum{h in H}F[h];
#minimize FO1: sum{j in VT} Peso2[j]*(beta[j]) + 100000*sum{h in H}F[h];
minimize FO: sum{j in VT} Peso2[j]*(C[j])  + 100000*sum{h in H}F[h] + sum{j in VT} 0.000001*beta[j];



#minimize pruebaz: sum{j in VT} Peso2[j]*(beta[j]) + 100000*sum{h in H}F[h] + sum{j in VT} 0.00000001*C[j];

subject to


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% SECUENCIA DE V�CTIMAS/VUELTAS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

# El punto de t�rmino de una vuelta debe ser igual al punto de inicio de la vuelta siguiente.
r100{m in M, v in V diff {VMAX}, h in H}: z[h,m,v] = sum{i in (VT union H)} x[h,i,m,(v+1)];

#Evita que existan recorridos innecesarios entre hospitales distintos.
r109{i in H, j in H, m in M, v in V: i <> j}: x[i,j,m,v] = 0;

#Todo veh�culo parte de su origen.
r2{m in M}: sum{j in (VT union H)} x[O[m],j,m,1] = 1;

# La recolecci�n de las v�ctimas debe ser sucesiva.
r3{a in VT, m in M, v in V}: ((sum{i in (VT union H): i <> a } x[i,a,m,v])-(sum{j in (VT union H):j<>a} x[a,j,m,v])) = 0;

#Toda vuelta de todo veh�culo debe terminar en un hospital.
r4{m in M, v in V}: sum{h in H} z[h,m,v] = 1;

#Las vueltas deben ser sucesivas y secuenciales.
r5{m in M, v in V diff {VMAX}}: u[m,v] >= u[m,(v+1)];

#IndiVT las vueltas en que efectivamente se trasladan v�ctimas.
r6{j in VT, m in M, v in V}: u[m,v] >= w[j,m,v];
r7{m in M, v in V}: u[m,v] <= sum{j in VT} w[j,m,v];

#Toda v�ctima debe ser atendida por un �nico veh�culo en una �niVT vuelta.
r8{j in VT}: sum{v in V, m in M} w[j,m,v] = 1;

#Evita que se recoja la misma v�ctima dentro de una vuelta.
r9{i in VT, m in M, v in V}: x[i,i,m,v] = 0;




#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% TIEMPOS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#VTlculo de recolecci�n de las v�ctimas (para la primera v�ctima de la primera vuelta).
r12{j in VT, m in M, h in H}: C[j] >= sum{e in E} (ME[m,e]*(TD[m]+TAT[e] + Sx[O[m],j,e])) + (ME[m,2]*t[j]+TP[j]) - M2 * (1 - w[j,m,1]) - M2 * ( 1 - x[O[m],j,m,1]);

#VTlculo de recolecci�n de las v�ctimas (cuando las v�ctimas pertenecen a una misma vuelta y las v�ctimas estan en el mismo nodo)
r13{i in VT, j in VT, m in M: PA[i] = PA[j]}: C[j] >= C[i] + ME[m,2]*(t[i] + t[j]) + (TP[j]) +  M2 * (sum{v in V} x[i,j,m,v] - 1);

#VTlculo de recolecci�n de las v�ctimas (cuando las v�ctimas pertenecen a una misma vuelta y las v�ctimas estan en distinto nodo).
r14{i in VT, j in VT, m in M: PA[i] <> PA[j]}: C[j] >= C[i] + ME[m,2]*(t[i] + t[j])+ sum{e in E} (ME[m,e]*Sx[i,j,e]) + sum{e in E:NA[i] <> NA[j]} (ME[m,e]*(TDS[e]+TAT[e])) + (TP[j]) + M2 * (sum{v in V} x[i,j,m,v] - 1);

#VTlculo de recolecci�n de las v�ctimas (cuando las v�ctimas NO pertenecen a una misma vuelta).
r15{j in VT, m in M, h in H, v in V diff {1}}: C[j] >= T[m,(v-1)] +  sum{e in E} (ME[m,e]*(TDS[e]+TAT[e]+Sx[h,j,e])) + (TP[j]) +  ME[m,2]*t[j] - M2 * (1 - w[j,m,v]) - M2 * ( 1 - z[h,m,(v-1)]);


#r135{i in VT, j in VT, m in M}: C[j] >= C[i] + sum{e in E: PA[i] <> PA[j]} (ME[m,e]*(TDS[e]+TAT[e]+Sx[i,j,e])) + sum{g in G} (GR[j,g]*TP[g]) + M2 * (sum{v in V} x[i,j,m,v] - 1);






#VTlculo los tiempos de recolecci�n para la primera vuelta.
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



#VTlculo de los tiempos de recolecci�n para VTda vuelta.
r17{v in V diff {1}, m in M}: T[m,v] = T[m,(v-1)] 
+ sum{j in VT} ((TP[j]+2*ME[m,2]*t[j])*w[j,m,v])
+ sum{i in (VT union H),j in (VT union H), e in E} (Sx[i,j,e] * ME[m,e] * x[i,j,m,v])
+ sum{e in E, i in VT,h in H} (ME[m,e]*(TDS[e]+TAT[e])*x[h,i,m,v])
+ sum{e in E, j in VT, i in VT: NA[i] <> NA[j] } (ME[m,e]*(TDS[e] + TAT[e])*x[i,j,m,v]) 
+ sum{j in VT, h in H, e in E} ((TDS[e] + TAT[e]) * ME[m,e] * x[j,h,m,v]) 
; 



#El c�lculo del tiempo s�lo puede ser realizado si la v�ctima j est� en movimiento.
r20{j in VT, m in M, v in V}: beta[j] >= T[m,v] - M2*(1-w[j,m,v]);


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% VTPACIDAD %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#VTpacidad de los hospitales fijos
r23{g in G, h in H diff{Hf}}: sum{m in M, j in VT, v in V} (y[j,h,m,v] * GR [j,g]) <= K[h,g];

#VTpacidad de los hospitales ficticios
r24{g in G, h in Hf}: sum{m in M, j in VT, v in V} (y[j,h,m,v] * GR [j,g]) <= K[h,g] * F[h];

#VTpacidad de los veh�culos
r25{m in M, v in V}: sum{j in VT} w[j,m,v] <= sum{e in E} (Q[e] * ME[m,e]);

#r235{g in G, h in H}: sum{m in M, j in VT, v in V} (y[j,h,m,v] * GR [j,g]) <= (K[h,g])* F[h] ;


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% HOSPITALES FICTICIOS Y SOBREVTPACIDAD %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#Activaci�n de hospitales ficticios
r26{g in G}: (sum{m in M, h in H, j in VT, v in V} (y[j,h,m,v] * GR[j,g])) - (sum{h in H} (K[h,g])) <= M2 * sum{h in Hf} F[h];

#L�mites de la sobreVTpacidad de los hospitales
#r27{g in G, h in H diff {Hf}}: L[h,g] <= R[h,g];
#r28{g in G, h in Hf}: L[h,g] <= R[h,g] * F[h];
#r275{g in G, h in H}: L[h,g] <= R[h,g];

#Las v�ctimas pueden ir a los hospitales ficticios s�lo si estos han sido activados.
r29{i in (VT union H), h in Hf, m in M, v in V}: x[i,h,m,v] <= F[h];
r30{i in (VT union H), h in Hf, m in M, v in V}: x[h,i,m,v] <= F[h];
#r295{i in VT, h in Hf, m in M, v in V}: x[h,i,m,v] + x[i,h,m,v] <= 2*F[h];




#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% RELACI�N ENTRE VARIABLES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


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