c
c     Source files for MENT2A extracted from locations given below
c     by Urs Rohrer (SIN), 1981
c
      FUNCTION ERFXX(X) 
C   
C     ******************************************************************    
C   
C     ENTRY POINTS ... ERF, ERFC, FREQ. 
C   
C     THESE THREE FUNCTIONS ARE COMPUTED FROM THE RATIONAL APPROXIMAT-  
C     IONS OF W.J.CODY, MATHEMATICS OF COMPUTATION, VOLUME 22 (1969),   
C     PAGES 631-637.    
C   
C     FOR ABS(X) LE 0.47 THE BASIC FUNCTION IS ERF.  FOR ABS(X) GT 0.47 
C     THE BASIC FUNCTION IS ERFC.  THE FINAL RESULT IS OBTAINED IN TERMS    
C     OF THE BASIC FUNCTION AS SHOWN IN THE FOLLOWING TABLE.  A=ABS(X)  
C     AND C=SQRT(2).    
C   
C       FUNCTION     A.LE.(0.47)                   A.GT.(0.47)  
C       REQUIRED       (ALL X)          (NEGATIVE X)       (POSITIVE X) 
C   
C       ERF(X)         ERF(X)             ERFC(A)-1          1-ERFC(X)  
C   
C       ERFC(X)       1-ERF(X)            2-ERFC(A)           ERFC(X)   
C   
C       FREQ(X)   0.5+0.5*ERF(X/C)      0.5*ERFC(A/C)    1-0.5*ERFC(X/C)    
C   
C     ******************************************************************    
C   
      DIMENSION P1(4),Q1(4), P2(8),Q2(8), P3(5),Q3(5)   
      DATA P1/2.42667 95523 E2,    
     1        2.19792 61618 E1,    
     2        6.99638 34886 E0,    
     3       -3.56098 43701 E-2/   
      DATA Q1/2.15058 87586 E2,    
     1        9.11649 05404 E1,    
     2        1.50827 97630 E1,    
     3        1.00000 00000 E0/    
      DATA P2/3.00459 26102 E2,    
     1        4.51918 95371 E2,    
     2        3.39320 81673 E2,    
     3        1.52989 28504 E2,    
     4        4.31622 27222 E1,    
     5        7.21175 82508 E0,    
     6        5.64195 51747 E-1,   
     7       -1.36864 85738 E-7/   
      DATA Q2/3.00459 26095 E2,    
     1        7.90950 92532 E2,    
     2        9.31354 09485 E2,    
     3        6.38980 26446 E2,    
     4        2.77585 44474 E2,    
     5        7.70001 52935 E1,    
     6        1.27827 27319 E1,    
     7        1.00000 00000 E0/    
      DATA P3/-2.99610 70770 E-3,  
     1        -4.94730 91062 E-2,  
     2        -2.26956 59353 E-1,  
     3        -2.78661 30860 E-1,  
     4        -2.23192 45973 E-2/  
      DATA Q3/1.06209 23052 E-2,   
     1        1.91308 92610 E-1,   
     2        1.05167 51070 E0,    
     3        1.98733 20181 E0,    
     4        1.00000 00000 E0/    
      DATA CONST1/ 0.70710 67811 /    
C        ( CONST1= SQRT(1/2) )  
      DATA CONST2/0.56418 95835 / 
C        ( CONST2= SQRT(1/PI) ) 
C   
C     ******************************************************************    
C   
C  ENTRY POINTS. SET IENTRY (=1 FOR ERF, =2 FOR ERFC, =3 FOR FREQ). 
C   
      ENTRY ERF(X)
      IENTRY=1  
      T=X   
      A=ABS(T)  
      IF(A.LE.6.0) GO TO 11 
      ERFXX=SIGN(1.0,X) 
      RETURN    
C   
      ENTRY ERFC(X)
      IENTRY=2  
      T=X   
      IF(T.GE.-6.0) GO TO 10    
      ERFXX=2.0 
      RETURN    
C   
      ENTRY FREQ(X)
      IENTRY=3  
      T=CONST1*X    
      IF(T.LE.6.0) GO TO 10 
      ERFXX=1.0 
      RETURN    
C   
C  SELECT BASIC FUNCTION. SET IBASIC (=1 FOR ERF, =2 FOR ERFC). 
C   
   10 A=ABS(T)  
   11 S=T**2    
      IF(A.GT.0.47) GO TO 20    
C   
C  IBASIC=1.  SET Y=ERF(T). 
C   
      IBASIC=1  
      Y=T*(P1(1)+S*(P1(2)+S*(P1(3)+S*P1(4) )))  
     1   /(Q1(1)+S*(Q1(2)+S*(Q1(3)+S*Q1(4) )))  
      GO TO 30  
C   
C  IBASIC=2. SET Y=ERFC(A). 
C   
   20 IBASIC=2  
      IF(A.GT.4.0) GO TO 21 
      Y=EXP(-S)*(P2(1)+A*(P2(2)+A*(P2(3)+A*(P2(4)+A*(P2(5)+ 
     1        A*(P2(6)+A*(P2(7)+A*P2(8) ))))))) 
     2         /(Q2(1)+A*(Q2(2)+A*(Q2(3)+A*(Q2(4)+A*(Q2(5)+ 
     3        A*(Q2(6)+A*(Q2(7)+A*Q2(8) ))))))) 
      GO TO 30  
C   
   21 Y=0.0 
      IF(A.GT.25.9969) GO TO 30 
      R=1.0/A   
      U=R**2    
      Y=R*EXP(-S)*( CONST2 +    
     1     U*(P3(1)+U*(P3(2)+U*(P3(3)+U*(P3(4)+U*P3(5) )))) 
     2      /(Q3(1)+U*(Q3(2)+U*(Q3(3)+U*(Q3(4)+U*Q3(5) )))) )   
C   
C  EXPRESS FINAL RESULT IN TERMS OF Y.  
C   
   30 IF(IENTRY.NE.1) GO TO 40  
      IF(IBASIC.EQ.2) GO TO 31  
      ERFXX=Y   
      RETURN    
   31 ERFXX=1.0-Y   
      IF(X.LT.0.0) ERFXX=-ERFXX 
      RETURN    
C   
   40 IF(IENTRY.NE.2) GO TO 50  
      IF(IBASIC.EQ.2) GO TO 41  
      ERFXX=1.0-Y   
      RETURN    
   41 ERFXX=Y   
      IF(X.LT.0.0) ERFXX=2.0-Y  
      RETURN    
C   
   50 R=0.5*Y   
      IF(IBASIC.EQ.2) GO TO 51  
      ERFXX=0.5+R   
      RETURN    
   51 ERFXX=1.0-R   
      IF(X.LT.0.0) ERFXX=R  
      RETURN    
      END   
