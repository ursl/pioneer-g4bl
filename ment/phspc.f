C
C       Special routine for MENT2A compiled by Urs Rohrer (1988)
C
	SUBROUTINE PHSPC(Z,IDX,N,M,ZPLAN,NC,DX,DY,SX,SY)
C
	REAL      LIM
	DIMENSION Z(IDX,1), ZPLAN(NC), SX(50), SY(50)
C
	DO 7 K  = 1, NC
	 LIM    = ZPLAN(K)
	 SX(K)  = 0.
	 SY(K)  = 0.
	 DO 6 I = 1, M
	  A = 0.
	  B = Z(1,I)
	  J = 1
    1	  CONTINUE
     	   IF (A.LE.LIM .AND. B.LE.LIM) GO TO 5
	   IF (A.LE.LIM .AND. B.GT.LIM) GO TO 2
	   IF (A.GT.LIM .AND. B.GT.LIM) GO TO 3
	   IF (A.GT.LIM .AND. B.LE.LIM) GO TO 4
    2	   SX(K) = SX(K) + (B-LIM) / (B-A)
	   SY(K) = SY(K) +.5 * (B+A) * (B-LIM) / (B-A)
	   GO TO 5
    3	   SX(K) = SX(K) + 1.
	   SY(K) = SY(K) + .5 * (B+A)
	   GO TO 5
    4	   SX(K) = SX(K) + (A-LIM) / (A-B)
	   SY(K) = SY(K) + .5 * (A+B) * (A-LIM) / (A-B)
    5	   J=J+1
	   IF (J .GT. N) GO TO 6
	   A = B
	   B = Z(J,I)
	  GO TO 1
    6	 CONTINUE
	 SX(K) = SX(K) * DX * DY * 10000.
	 IF (K .EQ. 1) REF = SY(1)
	 SY(K) = 100. * (SY(K) / REF)
    7	CONTINUE
C
	RETURN
	END
C
	SUBROUTINE CUTHFAC(HFAC,KM,I1,I2)
	DIMENSION HFAC(KM)
	I1=ABS(I1)
	I2=ABS(I2)
C
	IF (I1 .EQ. 0) GO TO 25
C
	DO 10 I=1,KM
	IF (HFAC(I) .NE. 0.) GO TO 15
   10	CONTINUE
   15	N=I-1
	DO 20 I=1,I1
   20	HFAC(N+I)=0.
C
   25	IF (I2 .EQ. 0) RETURN
C
	DO 30 I=KM,1,-1
	IF (HFAC(I) .NE. 0.) GO TO 35
   30	CONTINUE
   35	N=I+1
	DO 40 I=1,I2
   40	HFAC(N-I)=0.
	RETURN
	END
