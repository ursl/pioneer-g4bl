c
c       Subroutines for MENT2A compiled by Urs Rohrer (SIN), 1981 
c
	SUBROUTINE MAXV(X,IX,N,I,Y)
c
	DIMENSION X(200)
c
    	Y  = X(1)
	I = 1
	NN = (N-1)*IX+1
	DO 6 II = 1, NN, IX
	  Z = X(II)
    	  IF (Z-Y) 6,6,5
    5	  Y = Z
	  I = (II-1)/IX+1
    6	CONTINUE
	II = IX*(I-1)+1
	Y  = X(II)
	RETURN
	END

	SUBROUTINE MINV(X,IX,N,I,Y)
c
	DIMENSION X(200)
c
    	Y  = X(1)
	I  = 1
	NN = (N-1)*IX+1
	DO 6 II = 1, NN, IX
	  Z = X(II)
    	  IF (Z-Y) 5,6,6
    5	  Y = Z
	  I = (II-1)/IX+1
    6	CONTINUE
	II = IX*(I-1)+1
	Y  = X(II)
	RETURN
	END

	SUBROUTINE MAXAV(X,IX,N,I,Y)
c
	DIMENSION X(200)
c
    	Y  = X(1)
	I  = 1
	Y=ABS(Y)
	NN = (N-1)*IX+1
	DO 6 II = 1, NN, IX
	  Z = X(II)
	  Z = ABS(Z)
    	  IF (Z-Y) 6,6,5
    5	  Y = Z
	  I = (II-1)/IX+1
    6	CONTINUE
	II = IX*(I-1)+1
	Y  = X(II)
	RETURN
	END

	SUBROUTINE MINAV(X,IX,N,I,Y)
c
	DIMENSION X(200)
c
    	Y  = X(1)
	I  = 1
	Y=ABS(Y)
	NN = (N-1)*IX+1
	DO 6 II = 1, NN, IX
	  Z = X(II)
	  Z = ABS(Z)
    	  IF (Z-Y) 5,6,6
    5	  Y = Z
	  I = (II-1)/IX+1
    6	CONTINUE
	II = IX*(I-1)+1
	Y  = X(II)
	RETURN
	END

	SUBROUTINE MAXM(A,IA,N,M,I,J,B)
c
	DIMENSION A(IA,M)
c
    	B = A(1,1)
    	I = 1
	J = 1
c
	DO 1 K = 1, N
	 DO 2 L = 1, M
          S = A(K,L)
    	  IF (S-B) 99,99,5
    5	   B = S
	   I = K
	   J = L
   99	  CONTINUE
    2	 CONTINUE
    1   CONTINUE
	B = A(I,J)
	RETURN
	END

	SUBROUTINE MINM(A,IA,N,M,I,J,B)
c
	DIMENSION A(IA,M)
c
    	B = A(1,1)
    	I = 1
	J = 1
c
	DO 1 K = 1, N
	 DO 2 L = 1, M
	  S = A(K,L)
   	  IF (S-B) 5,99,99
    5	   B = S
	   I = K
	   J = L
   99	  CONTINUE
    2    CONTINUE
    1	CONTINUE
	B = A(I,J)
	RETURN
	END

	SUBROUTINE MAXAM(A,IA,N,M,I,J,B)
c
	DIMENSION A(IA,M)
c
    	B = ABS(A(1,1))
    	I = 1
	J = 1
c
	DO 1 K = 1, N
	 DO 2 L = 1, M
          S = ABS(A(K,L))
    	  IF (S-B) 99,99,5
    5	   B = S
	   I = K
	   J = L
   99	  CONTINUE
    2	 CONTINUE
    1   CONTINUE
	B = A(I,J)
	RETURN
	END

	SUBROUTINE MINAM(A,IA,N,M,I,J,B)
c
	DIMENSION A(IA,M)
c
    	B = ABS(A(1,1))
    	I = 1
	J = 1
c
	DO 1 K = 1, N
	 DO 2 L = 1, M
	  S = ABS(A(K,L))
   	  IF (S-B) 5,99,99
    5	   B = S
	   I = K
	   J = L
   99	  CONTINUE
    2    CONTINUE
    1	CONTINUE
	B = A(I,J)
	RETURN
	END

