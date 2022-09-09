c
c     Graphic Plot routines for MENT2A
c     Compiled and improved by Urs Rohrer (SIN/PSI) 1981-1995
c
      SUBROUTINE SPLOT
C
      COMMON / TROL / NAME,KTYPE,KADJ,MITER,PREC,QTOL,
     1                STEPAT,RELABN,RELRAN,STRATN
      CHARACTER*80    NAME
      COMMON          SPL(51),CONT(51),PFIT(51,20)
      COMMON / EXPD / KM,LM,PROJ(51,20),DRL(20),DS,REDS,SFID(20),LNOR
      COMMON / TRAN / IM,JM,XGRID(51),YGRID(51),SOURCE(51,51)
      COMMON / STOR / XCEN,YCEN,SCEN(20),ROMA(2,2,20),RINV(2,2,20),
     1                HFAC(51,20),SREF(20),SLIM,TLIM
      COMMON / PS /   PHSP(50),PERC(50),NOC,EMIT
      COMMON /CONTI/  HFACO(51,20), CONTF
      LOGICAL         CONTF, FIRST, FIRST2, FIRST3
      COMMON / TEKT / MINX,MAXX,MINY,MAXY,LCHX,LCHY,SCALX,SCALY
      REAL            MINX,MAXX,MINY,MAXY,LCHX,LCHY,SCALX,SCALY
      COMMON /COLTAG/ COLOR, XSCAL, YSCAL, XOFF, YOFF, CHSIZE, NDEV
      LOGICAL         COLOR
      REAL            WORK(51),SPL2(102),AUX(102)
      CHARACTER*13    LTIT,MTIT,STIT,TIT1,TIT2
      CHARACTER*30    INPUT
      CHARACTER*60    M1(10)
      CHARACTER*22    M2(3)
      CHARACTER*30    M3(14)
      CHARACTER*30    EMITEX, TEMP, PRINTCMD
      CHARACTER*40    TEXT1
      LOGICAL         HARDCOPY
c      DATA            MINX,MAXX,MINY,MAXY,LCHX,LCHY,SCALX,SCALY
c     1                / 0., 32.,  .5, 24., .32,  .6,  32.,  32./
      DATA            NDIMX / 51 /
      DATA            NDIMY / 51 /
      DATA            FIRST, FIRST2, FIRST3 /.TRUE.,.TRUE.,.TRUE./ 
      DATA            NCDEF /-110/
      DATA            LTIT /'L = 1       '/
      DATA            MTIT /'L = 1   MOD '/
      DATA            EMITEX /'Emit(86%)=110.00 Pi mmmrad    '/
      DATA            HARDCOPY /.FALSE./
      DATA            PRINTCMD /'./print 1 MENT.PS'/
C
      MINX  = 0.
      MAXX  = 32.
      MINY  = .5
      MAXY  = 24.
      LCHX  = .32
      LCHY  = .6
      SCALX = 32.
      SCALY = 32.
C
C     INITIALIZE POST SCRIPT
C
      XSCAL = 0.2
      YSCAL = 0.2
      XOFF  = 1.
      YOFF  = 1.
      CHSIZE = 8.
C
      M1(1) =' 0: Exit graphic mode'
      M1(2) =' 1: Profiles and corresponding projections of the source'
      M1(3) =' 2: H-factors for reconstructing the source'
      M1(4) =' 3: Source (contour plot) and phase space density'
      M1(5) =' 4: Source (3-dim plot)'
      M1(6) =' 5: Source (3-dim plot, turned 90 degs)'
      M1(7) =' 6: X -projection of the source'
      M1(8) =' 7: X''-projection of the source'
      M1(9) =' 8: Overview collection'
      M1(10)=' P: Hardcopy on printer'
C
      M2(1) = ' Printer Hardcopy    P'
      M2(2) = ' Continue         <CR>'
      M2(3) = ' Cancel Loop         C'
C
      M3(1)  = '  3 Contour lines w/o  labels'
      M3(2)  = '  5 Contour lines w/o  labels'
      M3(3)  = '  8 Contour lines w/o  labels'
      M3(4)  = ' 10 Contour lines w/o  labels'
      M3(5)  = ' 15 Contour lines w/o  labels'
      M3(6)  = ' 20 Contour lines w/o  labels'
      M3(7)  = ' 25 Contour lines w/o  labels'
      M3(8)  = '  3 Contour lines with labels'
      M3(9)  = '  5 Contour lines with labels'
      M3(10) = '  8 Contour lines with labels'
      M3(11) = ' 10 Contour lines with labels'
      M3(12) = ' 15 Contour lines with labels'
      M3(13) = ' 20 Contour lines with labels'
      M3(14) = ' 25 Contour lines with labels'
C
      DO 11 I = 80, 1, -1
       IF (NAME(I:I) .NE. ' ') GO TO 12
   11 CONTINUE
   12 MX = I
C
      DO 701 K=1,KM
       SPL(K)=DS*FLOAT(K-1)
       SPL2(K)=SPL(K)
       SPL2(KM+K)=SPL(K)
  701 CONTINUE
C
 1701 CONTINUE
      IF (FIRST2) WRITE(6,1005)
      FIRST2 = .FALSE.
      IF (FIRST3) THEN
       CALL INIXWI('TOMOGRAPHY'//char(0),5.,30.,1015.,800.)
       FIRST3 = .FALSE.
      ELSE
       CALL VT220_PUSH
      ENDIF
      CALL ERASE
      CALL SET_COL(1)	!	WEISS
      INPUT = '                              '
      CALL GETINP(10.,800.,
     *'Enter graphics frame number (1:8) or RET to terminate '//char(0)
     *,INPUT)
      CALL ERASE
      IF (INPUT(1:6) .EQ. 'MOUSE1') THEN
       READ(INPUT(7:20),'(2F7.1)') X1, Y1
       NX   = 57
       NY   = 8        ! ONLY M1(2) TO M1(9)
       LAN  = MENU(X1,Y1,NX,NY,M1(2))
       CALL ERASER(0.,760.,1060.,800.)
      ELSE IF (INPUT(1:9) .EQ. 'CONTROL_Z') THEN
       GO TO 801
      ELSE
       READ(INPUT(1:1),1001,ERR=1701) LAN
      ENDIF
      CALL SET_COL(3)	!	GRUEN
      GO TO 799
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
  700 CONTINUE
  702 CALL SET_COL(1)	!	WEISS
      INPUT = '                             '
      CALL ERASER(0.,760.,1060.,800.)
      CALL GETINP(10.,800.,
     * 'Enter number 1:8, P for Hardcopy or RET to terminate '//char(0),
     * INPUT)
      CALL SET_COL(3)	!	GRUEN
       IF (INPUT(1:6) .EQ. 'MOUSE1') THEN
        READ(INPUT(7:20),'(2F7.1)') X1, Y1
        NX   = 57
        NY   = 10
        LANX = MENU(X1,Y1,NX,NY,M1) - 1
	IF (LANX .EQ. 9) THEN       ! HARDCOPY
	 INPUT(1:1) = 'P'
	ELSE
	 WRITE(INPUT(1:1),'(I1)') LANX
	ENDIF
       ENDIF
       IF (INPUT(1:1).EQ.'P' .OR. INPUT(1:1).EQ.'p') THEN
	IF (.NOT. HARDCOPY) CALL PSOPEN('MENT.PS')
        HARDCOPY = .TRUE.
        CALL SPLOTPS(LAN,L)
        GO TO 702
       ELSE IF (INPUT(1:9) .EQ. 'CONTROL_Z') THEN
        GO TO 801
       ELSE
        READ(INPUT(1:1),1001,ERR=702) LAN
       ENDIF
      CALL ERASE
  799 IF (LAN .EQ. 0) GO TO 801
      IF (LAN.LT.0 .OR. LAN.GT.8) GO TO 702
      GOTO (703,704,705,706,707,708,709,710), LAN
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
  703 DO 713 L=1,LM
      WRITE(LTIT(4:5),973) L
      LTIT(6:6) = CHAR(0)
      DO 1703 K=1,KM
       AUX(K)=PROJ(K,L)
       AUX(KM+K)=PFIT(K,L)
 1703 CONTINUE
      CALL SET_COL(3)	!	GRUEN
      CALL TPLOT(SPL2,AUX,2*KM,1,-1.,-1.,'S (cm)'//char(0),6,
     1'Proj'//char(0),4,LTIT(1:5)//char(0),5,1,1,-1,0,1,1,1.
     2,0,0,0,0,0,0)
      CALL SET_COL(5)	!	HELL-BLAU
      CALL TPLOT(SPL,PROJ(1,L),KM,0,-1.,-1.,'S (cm)'//char(0),6,
     1'Proj'//char(0),4,LTIT(1:5)//char(0),5,1,1,-1,8,1,1,1.
     2,0,0,0,0,0,0)
      CALL SET_COL(1)	!	WEISS
      CALL TPLOT(SPL,PFIT(1,L),KM,0,-1.,-1.,'S (cm)'//char(0),6,
     1'Proj'//char(0),4,LTIT(1:5)//char(0),5,1,1,0,0,1,1,1.
     2,0,0,0,0,0,0)
      CALL SET_COL(3)	!	GRUEN
      IF (L .EQ. LM) GO TO 702
      CALL SET_COL(1)	!	WEISS
      INPUT = '                             '
      CALL GETINP(10.,800.,
     *'Enter RET for continue or P for Hardcopy '//char(0),INPUT)
      CALL SET_COL(3)	!	GRUEN
       IF (INPUT(1:6) .EQ. 'MOUSE1') THEN
        READ(INPUT(7:20),'(2F7.1)') X1, Y1
        NX    = 22
        NY    = 3
        NN    = MENU(X1,Y1,NX,NY,M2)
	IF (NN .EQ. 1) INPUT(1:1) = 'P'
	IF (NN .EQ. 2) INPUT(1:1) = ' '
	IF (NN .EQ. 3) INPUT(1:1) = '5'
       ELSE IF (INPUT(1:6) .EQ. 'MOUSE2') THEN
	INPUT(1:1) = ' '
       ENDIF
       IF (INPUT(1:1).EQ.'P' .OR. INPUT(1:1).EQ.'p') THEN
	IF (.NOT. HARDCOPY) CALL PSOPEN('MENT.PS')
        HARDCOPY = .TRUE.
        CALL SPLOTPS(LAN,L)
       ELSE IF (INPUT(1:9) .EQ. 'CONTROL_Z') THEN
        GO TO 801
       ELSE
        READ(INPUT(1:1),1001,ERR=700) IDUM
        IF (IDUM .NE. 0) GO TO 700
       ENDIF
      CALL ERASE
  713 CONTINUE
      GO TO 700
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
  704 DO 714 L=1,LM
      WRITE(LTIT(4:5),973) L
      LTIT(6:6) = CHAR(0)
      STIT = LTIT
      INDS = 5
  712 CALL SET_COL(1)	!	WEISS	
      CALL TPLOT(SPL,HFAC(1,L),KM,1,-1.,-1.,'S (cm)'//char(0),6,
     1'HFAC  '//char(0),6,STIT(1:INDS)//char(0),INDS,1,1,-1,4,1,1,1.
     2,0,0,0,0,0,0)
      CALL SET_COL(1)	!	WEISS
      INPUT = '                              '
      CALL GETINP(10.,800.,
     *'Enter IcutL,IcutR, P for Hardcopy or RET for continue '//char(0)
     *,INPUT)
      CALL SET_COL(3)	!	GRUEN
       IF (INPUT(1:6) .EQ. 'MOUSE1') THEN
        READ(INPUT(7:20),'(2F7.1)') X1, Y1
        NX    = 22
        NY    = 3
        NN    = MENU(X1,Y1,NX,NY,M2)
	IF (NN .EQ. 1) INPUT(1:1)  = 'P'
	IF (NN .EQ. 2) INPUT(1:10) = '          '
	IF (NN .EQ. 3) INPUT(1:1)  = 'Y'
       ELSE IF (INPUT(1:6) .EQ. 'MOUSE2') THEN
	INPUT(1:10) = '          '
       ENDIF
       IF (INPUT(1:1).EQ.'P' .OR. INPUT(1:1).EQ.'p') THEN
	IF (.NOT. HARDCOPY) CALL PSOPEN('MENT.PS')
        HARDCOPY = .TRUE.
        CALL SPLOTPS(LAN,L)
       ELSE IF (INPUT(1:9) .EQ. 'CONTROL_Z') THEN
        GO TO 801
       ELSE
        READ(INPUT,1004,ERR=700) ICUT1,ICUT2
         IF (ICUT1.NE.0 .OR. ICUT2.NE.0) THEN
          CALL CUTHFAC(HFAC(1,L),KM,ICUT1,ICUT2)
          WRITE(MTIT(4:5),973) L
	  MTIT(13:13) = CHAR(0)
          STIT = MTIT
	  INDS = 11
          CALL ERASE
          GO TO 712
         ENDIF
       ENDIF
      CALL ERASE
  714 CONTINUE
      GO TO 700
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
  705 IF (FIRST) WRITE(6,1006)
      FIRST=.FALSE.
      TEXT1 = 'Enter number of contour lines (D=  20) '//CHAR(0)
      WRITE(TEXT1(34:37),1009) NCDEF
      INPUT = '                               '
      CALL SET_COL(1)	!	WEISS
      CALL GETINP(10.,800.,TEXT1//char(0),INPUT)
      CALL SET_COL(3)	!	GRUEN
      CALL ERASE
      IF (INPUT(1:6) .EQ. 'MOUSE1') THEN
       READ(INPUT(7:20),'(2F7.1)') X1, Y1
       NX    = 29
       NY    = 14
       NN    = MENU(X1,Y1,NX,NY,M3)
       IF (NN .EQ. 1)  NCONT = -103
       IF (NN .EQ. 2)  NCONT = -105
       IF (NN .EQ. 3)  NCONT = -108
       IF (NN .EQ. 4)  NCONT = -110
       IF (NN .EQ. 5)  NCONT = -115
       IF (NN .EQ. 6)  NCONT = -120
       IF (NN .EQ. 7)  NCONT = -125
       IF (NN .EQ. 8)  NCONT =  103
       IF (NN .EQ. 9)  NCONT =  105
       IF (NN .EQ. 10) NCONT =  108
       IF (NN .EQ. 11) NCONT =  110
       IF (NN .EQ. 12) NCONT =  115
       IF (NN .EQ. 13) NCONT =  120
       IF (NN .EQ. 14) NCONT =  125
      ELSE IF (INPUT(1:6) .EQ. 'MOUSE2') THEN
       NCONT = 0	!	TAKE DEFAULT
      ELSE IF (INPUT(1:9) .EQ. 'CONTROL_Z') THEN
       GO TO 801
      ELSE IF (INPUT(2:2) .EQ. ' ') THEN
       READ(INPUT(1:1),'(I1)',ERR=700) NCONT
      ELSE IF (INPUT(3:3) .EQ. ' ') THEN
       READ(INPUT(1:2),'(I2)',ERR=700) NCONT
      ELSE IF (INPUT(4:4) .EQ. ' ') THEN
       READ(INPUT(1:3),'(I3)',ERR=700) NCONT
      ELSE
       READ(INPUT(1:4),'(I4)',ERR=700) NCONT
      ENDIF
      IF (NCONT .EQ. 0) NCONT = NCDEF
      IF (NCONT .NE. 0) NCDEF = NCONT
      WX = XGRID(IM) - XGRID(1)
      WY = YGRID(JM) - YGRID(1)
      CALL MODXSCAL(XGRID,IM,JM,SOURCE)
      NPX = -IM
      NPY = -JM
      CALL TCONTR(XGRID,NPX,YGRID,NPY,SOURCE,NDIMX,NDIMY,NCONT,
     1 0.,1.,.1,CONT,WX,WY,'S (cm)'//char(0),6,'T (r) '//char(0),5,
     2 NAME(1:MX)//char(0),MX,1.,0,0,0,0,0,0)
      CALL RESXSCAL(XGRID,IM,JM,SOURCE)
      CALL SET_COL(1)	!	WEISS
      INPUT = '                             '
      CALL GETINP(10.,800.,
     * 'Enter RET for continue or P for Hardcopy '//char(0),INPUT)
      CALL SET_COL(3)	!	GRUEN
       IF (INPUT(1:6) .EQ. 'MOUSE1') THEN
        READ(INPUT(7:20),'(2F7.1)') X1, Y1
        NX    = 22
        NY    = 3
        NN    = MENU(X1,Y1,NX,NY,M2)
	IF (NN .EQ. 1) INPUT(1:1) = 'P'
	IF (NN .EQ. 2) INPUT(1:1) = ' '
	IF (NN .EQ. 3) INPUT(1:1) = 'Y'
       ELSE IF (INPUT(1:6) .EQ. 'MOUSE2') THEN
	INPUT(1:1) = ' '
       ENDIF
       IF (INPUT(1:1).EQ.'P' .OR. INPUT(1:1).EQ.'p') THEN
        L = 0
	IF (.NOT. HARDCOPY) CALL PSOPEN('MENT.PS')
        HARDCOPY = .TRUE.
        CALL SPLOTPS(LAN,NCONT)
       ELSE IF (INPUT(1:9) .EQ. 'CONTROL_Z') THEN
        GO TO 801
       ELSE
        READ(INPUT(1:1),1001,ERR=700) IDUM
        IF (IDUM .NE. 0) GO TO 700
       ENDIF
      CALL ERASE
      CALL SET_COL(2)	!	ROT	
      CALL TPLOT(PHSP,PERC,NOC-1,1,-1.,-1.,'* Pi (mmmrad)'//char(0),14,
     1'100-%'//char(0),5,NAME(1:MX)//char(0),MX,1,2,0,2,1,1,1.
     2,0,0,0,0,0,0)
      CALL SET_COL(3)	!	GRUEN	
      L = NCONT
      LAN = 100
      GO TO 702
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
  706 CALL SET_COL(5)	!	HELL-BLAU	
      CALL MODXSCAL2(XGRID,IM,JM,SOURCE)
      CALL TISO3D(SOURCE,NDIMX,IM,JM,1,CONT,NAME(1:MX)//char(0)
     X,MX,0,0)
      CALL RESXSCAL2(XGRID,IM,JM,SOURCE)
      CALL SET_COL(3)	!	GRUEN	
      GO TO 702
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
  707 CALL SET_COL(5)	!	HELL-BLAU	
      CALL MODXSCAL2(XGRID,IM,JM,SOURCE)
      CALL TISO3D(SOURCE,NDIMX,IM,JM,0,CONT,NAME(1:MX)//char(0)
     X,MX,0,0)
      CALL RESXSCAL2(XGRID,IM,JM,SOURCE)
      CALL SET_COL(3)	!	GRUEN	
      GO TO 702
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
  708 DO 510 J=1,51
      WORK(J)=0.
      DO 500 I=1,51
  500 WORK(J)=WORK(J)+SOURCE(J,I)
  510 WORK(J)=WORK(J)*(YGRID(2)-YGRID(1))
      CALL SET_COL(1)	!	WEISS
      CALL TPLOT(XGRID,WORK,KM,1,-1.,-1.,'X (cm)'//char(0),6,
     1'Proj'//char(0),4,NAME(1:MX)//char(0),MX,1,1,0,0,1,1,1.
     2,0,0,0,0,0,0)
      CALL SET_COL(3)	!	GRUEN	
      GO TO 702
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
  709 YMAX = 0.
       DO 530 J=1,51
        WORK(J)=0.
        DO 520 I=1,51
  520  WORK(J)=WORK(J)+SOURCE(I,J)
  530  YMAX=MAX(YMAX,WORK(J))
       DO 540 J=1,51
  540  WORK(J)=WORK(J)/YMAX
      CALL SET_COL(1)	!	WEISS
      CALL TPLOT(YGRID,WORK,KM,1,-1.,-1.,'T (r)'//char(0),5,
     1'Proj'//char(0),4,NAME(1:MX)//char(0),MX,1,1,0,0,1,1,1.
     2,0,0,0,0,0,0)
      CALL SET_COL(3)	!	GRUEN	
      GO TO 702
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
  710 CALL TEXT_OUT(NAME(1:MX)//char(0),3.*SCALX,23.*SCALY,2)
      DO 2713 L=1,MIN(6,LM)
       CALL FRAME(L)
       IF (L.LE.3) THEN
        TIT2 = 'P'//CHAR(0)
       ELSE
	TIT2 = ' '//CHAR(0)
       ENDIF
       IF (L.EQ.3 .OR. L.EQ.MIN(6,LM)) THEN
        TIT1 = 'S (cm)'//CHAR(0)
       ELSE
        TIT1 = ' '//CHAR(0)
       ENDIF
       WRITE(LTIT(4:5),973) L
       LTIT(6:6) = CHAR(0)
       DO 2703 K=1,KM
        AUX(K)=PROJ(K,L)
        AUX(KM+K)=PFIT(K,L)
 2703  CONTINUE
      CALL SET_COL(3)	!	GRUEN	
      CALL TPLOT(SPL2,AUX,2*KM,1,-1.,-1.,TIT1//char(0),6,
     1 TIT2//char(0),1,LTIT(1:5)//char(0),5,1,1,-1,0,1,1,1.
     2,0,0,0,0,0,1)
      CALL SET_COL(5)	!	HELL-BLAU
      CALL TPLOT(SPL,PROJ(1,L),KM,0,-1.,-1.,TIT1//char(0),6,
     1 TIT2//char(0),1,LTIT(1:5)//char(0),5,1,1,-1,8,1,1,1.
     2,0,0,0,0,0,1)
      CALL SET_COL(1)	!	WEISS	
      CALL TPLOT(SPL,PFIT(1,L),KM,0,-1.,-1.,TIT1//char(0),6,
     1 TIT2//char(0),1,LTIT(1:5)//char(0),5,1,1,0,0,1,1,1.,
     20,0,0,0,0,1)
      CALL SET_COL(3)	!	GRUEN	
 2713 CONTINUE
      NCONT=NCDEF
      WX=XGRID(IM)-XGRID(1)
      WY=YGRID(JM)-YGRID(1)
      CALL MODXSCAL(XGRID,IM,JM,SOURCE)
      NPX=-IM
      NPY=-JM
      CALL FRAME(7)
      CALL TCONTR(XGRID,NPX,YGRID,NPY,SOURCE,NDIMX,NDIMY,NCONT,
     1 0.,1.,.1,CONT,WX,WY,'S (cm)'//char(0),6,'T (r) '//char(0),6,
     2 NAME(1:MX)//char(0),MX,1.,0,0,0,0,1,1)
      CALL RESXSCAL(XGRID,IM,JM,SOURCE)
      CALL FRAME(8)
      CALL SET_COL(2)	!	ROT	
      CALL TPLOT(PHSP,PERC,NOC-1,1,-1.,-1.,'* Pi (mmmrad)'//char(0),14,
     1 '100-%'//char(0),5,NAME(1:MX)//char(0),MX,1,2,0,2,1,1,1.
     2,0,0,0,0,1,1)
      WRITE(EMITEX(11:16),1010) EMIT
      TEMP(1:11) = EMITEX(1:10)//CHAR(0)
      CALL TEXT_OUT(TEMP,29.2*SCALX,9.5*SCALY,2)
      N = 9
      CALL MYTRIM(EMITEX(11:19),TEMP,N)
      TEMP(N+1:N+1) = CHAR(0)
      CALL TEXT_OUT(TEMP,29.2*SCALX,9.0*SCALY,2)
      TEMP(1:7) = EMITEX(21:26)//CHAR(0)
      CALL TEXT_OUT(TEMP,29.2*SCALX,8.5*SCALY,2)
      CALL FRAME(9)
      CALL SET_COL(5)	!	HELL-BLAU
      CALL MODXSCAL2(XGRID,IM,JM,SOURCE)
      CALL TISO3D(SOURCE,NDIMX,IM,JM,0,CONT,NAME(1:MX)//char(0)
     X,MX,1,1)
      CALL RESXSCAL2(XGRID,IM,JM,SOURCE)
      CALL FRAME(0)
      CALL SET_COL(1)	!	WEISS		
      CALL TIMDS(29.,.1)
      GO TO 702
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
  801 CALL VT220_POP
      IF (HARDCOPY) THEN
       CALL PSCLS
       WRITE(PRINTCMD(9:9),'(I1)') NDEV
       CALL SYSTEM(PRINTCMD)
       HARDCOPY = .FALSE.
      ENDIF
      RETURN
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
  973 FORMAT(I2)
 1000 FORMAT(
     X' Enter frame number (1:8), P for Printronix-Copy or RET
     X to terminate ',$)
 1001 FORMAT(I1)
 1002 FORMAT(' Enter RET for continue or P for Printronix-Copy ',$)
 1003 FORMAT(' Enter number of contour lines (D=',I4,') ',$)
 1004 FORMAT(I1,1X,I1)
 1005 FORMAT(' Frame numbers are:'/
     1' 1: Profiles and corresponding projections of the source'/
     2' 2: H-factors for reconstructing the source'/
     3' 3: Source (contour plot) and phase space density'/
     4' 4: Source (3-dim plot)'/
     5' 5: Source (3-dim plot, turned 90 degs)'/ 
     6' 6: X -projection of the source'/
     7' 7: X''-projection of the source'/
     8' 8: Overview collection')
 1006 FORMAT('+Number of contour lines: 3 to 25'/ 
     1' Add 100 for nonlinear scale in z-direction'/ 
     2' Multiply with -1 for suppressing labels')
 1007 FORMAT(' Enter IcutL,IcutR, P for Printronix-Copy or RET
     X for continue  ',$)
 1008 FORMAT(
     X' Enter graphics frame number (1:8) or RET to terminate ',$)
 1009 FORMAT(I4)
 1010 FORMAT(F6.2)
 1011 FORMAT(I2)
 1200 FORMAT(A)
      END

      SUBROUTINE TCONTR(X,NPX,Y,NPY,Z,IDX,IDY,NC,ZMN,ZMX,DLZ,ZPLAN,
     1 XAXIS,YAXIS,LABELX,NLX,LABELY,NLY,LABELZ,NLZ,SYSIZE,KXDEN,
     2 KYDEN,KXTICS,KYTICS,NOTIT,NOTDS)
C
      COMMON /CNTRCOM/ ISYM(50)
      COMMON / TEKT / MINX,MAXX,MINY,MAXY,LCHX,LCHY,SCALX,SCALY
      REAL            MINX,MAXX,MINY,MAXY,LCHX,LCHY,SCALX,SCALY
      COMMON / DEV /  ITYP,IOPT
      COMMON / CSYMB / NOSYM
      COMMON / PS /   PHSP(50),PERC(50),NOC,EMIT
      CHARACTER*(*)   LABELX, LABELY, LABELZ
C      CHARACTER*28    LEGEND
      CHARACTER*8     LEGEND1
      CHARACTER*7     LEGEND2
      REAL            XSCALE(2), YSCALE(2)
      REAL            X(2), Y(2), Z(IDX,1), ZPLAN(1)
      REAL            ZT(4), MAXXOLD
C
      MAXXOLD = MAXX
      MAXX    = MAXX - 20 * LCHX
C
      NOSYM   = 1
      IF (NC .LT. 0) NOSYM = 0
      NOC     = IABS(NC)
      IF (NOC .GT. 100) NOC = NOC - 100
      NOC     = MAX(NOC,3)
      NOC     = MIN(NOC,25)
C
      ZMIN    = ZMN
      ZMAX    = ZMX
      DELZ    = 0.0
      NOX     = IABS(NPX)
      NOY     = IABS(NPY)
      DO 10 I = 1, 50
   10 ISYM(I) = 0
C     ESTABLISH SCALES
      CALL MINV (X,1,NOX,I,XSCALE(1))
      CALL MAXV (X,1,NOX,I,XSCALE(2))
      CALL MINV (Y,1,NOY,I,YSCALE(1))
      CALL MAXV (Y,1,NOY,I,YSCALE(2))
      CALL SET_COL(3)	!	GRUEN	
      CALL TPLOT(XSCALE,YSCALE,2,1,-1.,-1.,LABELX,NLX,
     1 LABELY,NLY,LABELZ,NLZ,1,1,-1,0,1,1,1.
     2,KXDEN,KYDEN,KXTICS,KYTICS,NOTIT,NOTDS)
      IF (NPX .LE. 0) CALL MINM (Z,IDX,NOX,NOY,I,J,ZMIN)
      IF (NPY .LE. 0) CALL MAXM (Z,IDX,NOX,NOY,I,J,ZMAX)
      IF (DELZ .GT. 0) GO TO 20
      DELZ = (0.99*ZMAX-ZMIN)/(NOC-1.)
   20 IF (IDY .GT. 0) GO TO 30
      ZMAX = ZMAX-AMOD(ZMAX,DELZ)
      ZMIN = ZMIN-AMOD(ZMIN,DELZ)
      NOC  = MIN(NOC,IFIX((ZMAX-ZMIN)/DELZ+1.01))
   30 ZPLAN(1) = ZMIN
      DO 40 I = 2, NOC
      ZPLAN(I) = ZPLAN(I-1) + DELZ
   40 CONTINUE
      IF (ABS(NC) .LT. 100) GOTO 55
      DO 50 I = 2, NOC
      VAR = FLOAT(I-1)/FLOAT(NOC-1)
      VAR = VAR * VAR
      ZPLAN(I) = ZMIN+(.99*ZMAX-ZMIN)*VAR*(.25+.75*VAR*VAR)
   50 CONTINUE
   55 CONTINUE
      DO 90 NY = 2, NOY
      IX = MOD(NY,2)
      DY = Y(NY)-Y(NY-1)
      DO 80 INX = 2, NOX
      NX = INX
      IF (IX .NE. 0) NX = NOX - INX + 2
      ZT(1) = Z(NX-1,NY-1)
      ZT(2) = Z(NX,NY-1)
      ZT(3) = Z(NX,NY)
      ZT(4) = Z(NX-1,NY)
      DX    = X(NX)-X(NX-1)
      IF (ABS(ZT(3)-ZT(1))-ABS(ZT(4)-ZT(2))) 70,60,60
   60 CALL TRICT(X(NX),Y(NY),-DX,-DY,NOC,ZPLAN,ZT(4),ZT(3),ZT(2))
      CALL TRICT(X(NX-1),Y(NY-1),DX,DY,NOC,ZPLAN,ZT(2),ZT(1),ZT(4))
      GO TO 80
   70 CALL TRICT(X(NX-1),Y(NY),DX,-DY,NOC,ZPLAN,ZT(3),ZT(4),ZT(1))
      CALL TRICT(X(NX),Y(NY-1),-DX,DY,NOC,ZPLAN,ZT(1),ZT(2),ZT(3))
   80 CONTINUE
   90 CONTINUE
      XS = MAXX + 4. * LCHX
      YS = MAXY - 4. * LCHY
      CALL SET_COL(1)	!	WEISS	
      IF (NOTIT.EQ.0) CALL TEXT_OUT('Cont  mmmrad   %'//char(0),
     *                (XS-3.*LCHX)*SCALX,YS*SCALY,INT(10.*LCHX)-1)
      YS  = YS - LCHY
      CALL PHSPC(Z,IDX,NOX,NOY,ZPLAN,NOC,X(2)-X(1),Y(2)-Y(1),PHSP,PERC)
      DO 120 I = 1, NOC
       YS  = YS - LCHY
       LEI = 64 + I

       WRITE(LEGEND1(1:8),9102,ERR=110) PHSP(I)
 9102  FORMAT(F8.1)
       GO TO 111
  110  WRITE(LEGEND1(1:8),9103) PHSP(I)
 9103  FORMAT(1PE8.2)
  111  CONTINUE

       WRITE(LEGEND2(1:7),9112,ERR=112) PERC(I)
 9112  FORMAT(F7.1)
       GO TO 113
  112  WRITE(LEGEND2(1:7),9113) PERC(I)
 9113  FORMAT(0PF7.1)
  113  CONTINUE

       IF (NOTIT .EQ. 0) THEN
        CALL TEXT_OUT(CHAR(LEI)//':'//CHAR(0),(XS-2.*LCHX)*SCALX,
     *                YS*SCALY,INT(10.*LCHX)-1)
        CALL TEXT_OUT(LEGEND1//CHAR(0),        XS*SCALX,
     *                YS*SCALY,INT(10.*LCHX)-1)
        CALL TEXT_OUT(LEGEND2//CHAR(0),       (XS+6.*LCHX)*SCALX,
     *                YS*SCALY,INT(10.*LCHX)-1)
       ENDIF
  120 CONTINUE
      DO 200 J  = 2, NOC
       PHSP(J-1) = PHSP(J)/3.1416
       PERC(J-1) = 100.-PERC(J)
  200 CONTINUE
      EMIT = 0.0
      DO 210 J = 1, NOC-1
       IF (PERC(J) .GE. 14.0) GO TO 220
  210 CONTINUE
      GO TO 230
  220 DIF1 = ALOG(PERC(J)) - ALOG(PERC(J-1))
      DIF2 = ALOG(14.0)    - ALOG(PERC(J-1))	!  86%-INTERSECTION
      IF (DIF1 .EQ. 0.0) GO TO 230
      Q    = DIF2 / DIF1
      DIF3 = PHSP(J-1) - PHSP(J)
      EMIT = PHSP(J-1) - Q * DIF3		! 4*SIGMA EMITTANCE
230   MAXX      = MAXXOLD
      CALL FLUSH_XWI
      RETURN
C
      END

      SUBROUTINE TISO3D(A,IA,NX,NY,LFLIP,COVER,LABEL,NL,NOTIT,NOTDS)
C
      COMMON / TEKT / MNU,MXU,MNV,MXV,LCHU,LCHV,SCALX,SCALY
      REAL            MNU,MXU,MNV,MXV,LCHU,LCHV,SCALX,SCALY
      REAL            LABH,LABW,LEGW
      DIMENSION       A(51,51),COVER(1)
      CHARACTER*(*)   LABEL
      LOGICAL         COVL,COVR
C
C     INLINE FUNCTION DEFINITION FOR SCALING DATA FROM ARRAY A
C
      VALUE(I,J) = RANGE*A(I+KFLIP*(NU+1-2*I),J+KFLIP*(NV+1-2*J))
C
      KFLIP  = LFLIP
      IF (KFLIP .NE. 0) KFLIP = 1
      RATIO  = 1./SQRT(3.)
C
C     DETERMINE MIN AND MAX VALUE OF ARRAY, FIX DELTAU,DELTAV
C
      CALL MAXM(A,IA,NX,NY,IHI,JHI,AHI)
      NU     = MAX(3,NX)
      NV     = MAX(3,NY)
      LABH   = 2.*LCHV
      LABW   = FLOAT(NL)*LCHU
      LEGW   = 2.*LCHU
      UWID   = MXU-MNU-2.*LEGW
      VWID   = MXV-MNV-2.*LABH
      SCALE  = MIN(UWID,VWID/(2.*RATIO))
      DELTAU = SCALE/(2.*NV)
      DELTAV = DELTAU*RATIO
      ROOF   = SCALE*RATIO
      RANGE  = ROOF/AHI
C
C     U0 AND V0 ARE COORDINATES OF THE ARRAY ORIGIN
C
      U0 = (MXU + MNU) / 2.
      V0 = MNV
C
C     DISPLAY FIRST LINE, INITIALIZE COVER
C
      CALL GRAMOV(SCALX*U0,SCALY*V0)
      DO 101 IU  = 1, NU
       U  = U0+DELTAU*(IU-1)
       V  = V0+DELTAV*(IU-1)
       Z  = V+VALUE(IU,1)
       COVER(IU) = Z
       CALL GRADRW(SCALX*U,SCALY*Z)
  101 CONTINUE
      CALL GRADRW(SCALX*U,SCALY*V)
C
C     DISPLAY OTHER LINES, UPDATE COVER
C
      DO 401 IV = 2, NV
       U    = U0-DELTAU*FLOAT(IV-1)
       V    = V0+DELTAV*FLOAT(IV-1)
       CALL GRAMOV(SCALX*U,SCALY*V)
       COLD = V+VALUE(1,IV)
       CALL GRADRW(SCALX*U,SCALY*COLD)
       U    = U+DELTAU
       V    = V+DELTAV
       BR   = V+VALUE(2,IV)
       CALL GRADRW(SCALX*U,SCALY*BR)
       FR   = COVER(1)
       COVR = FR .GT. BR
       DO 301 IU = 3, NU
        U    = U0+DELTAU*(IU-IV)
        V    = V0+DELTAV*(IU+IV-2)
        FL   = FR
        FR   = COVER(IU-1)
        BL   = BR
        BR   = V+VALUE(IU,IV)
        COVL = COVR
        COVR = FR .GT. BR
C
C       DISPLAY APPROPRIATE SEGMENT FOR 4 POSSIBILITIES
C
        IF ((COVL .AND. .NOT.COVR) .OR. (COVR. AND. .NOT.COVL)) THEN
         SB  = BR-BL
         SF  = FR-FL
         DCO = (BL-FL)/(SF-SB)
         DCU = DCO*DELTAU
         DCV = DCO*SB
	ENDIF
	IF (COVL .AND. COVR) THEN
	 CONTINUE
        ELSE IF (COVL) THEN
         CALL GRAMOV(SCALX*(U-DELTAU+DCU),SCALY*(BL+DCV))
         CALL GRADRW(SCALX*U,SCALY*BR)
        ELSE IF (COVR) THEN
         CALL GRADRW(SCALX*(U-DELTAU+DCU),SCALY*(BL+DCV))
	ELSE
         CALL GRADRW(SCALX*U,SCALY*BR)
	ENDIF
        COVER(IU-2) = COLD
        COLD = MAX(BL,FL)
  301  CONTINUE
       IF (.NOT.COVR) CALL GRADRW(SCALX*U,SCALY*AMAX1(V,FR))
       COVER(NU-1) = COLD
  401 CONTINUE
C
C     DRAW WALLS OF BOX
C
      CALL SET_COL(3)	!	GRUEN
      UU = DELTAU*FLOAT(NU-1)
      UV = DELTAU*FLOAT(NV-1)
      VU = DELTAV*FLOAT(NU-1)
      VV = DELTAV*FLOAT(NV-1)
      IF (NOTIT .EQ. 0) THEN
       CALL GRAMOV(SCALX*U0,SCALY*V0)
       CALL GRADRW(SCALX*(U0+UU),SCALY*(V0+VU))
       CALL GRADRW(SCALX*(U0+UU),SCALY*(V0+VU+ROOF))
       IF (KFLIP .NE. 1) THEN
        CALL GRADRW(SCALX*U0,SCALY*(V0+ROOF))
        CALL GRADRW(SCALX*U0,SCALY*V0)
        CALL GRAMOV(SCALX*U0,SCALY*(V0+ROOF))
       ELSE
        CALL GRADRW(SCALX*(U0+UU-UV),SCALY*(V0+VU+VV+ROOF))
        CUTL = MAX(BR,FR)
        CALL GRADRW(SCALX*(U0+UU-UV),SCALY*CUTL)
        CALL GRAMOV(SCALX*(U0+UU-UV),SCALY*(V0+VU+VV+ROOF))
       ENDIF
       CALL GRADRW(SCALX*(U0-UV),SCALY*(V0+VV+ROOF))
       CALL GRADRW(SCALX*(U0-UV),SCALY*(V0+VV))
       CALL GRADRW(SCALX*U0,SCALY*V0)
      ENDIF
C
C     WRITE LABEL
C
      UXL = MAX(0.,U0-FLOAT(NL/2)*LCHU)
      IF (NOTIT .EQ. 0) CALL TEXT_OUT(LABEL,UXL*SCALX,
     *                  (MXV-LABH)*SCALY,INT(10.*LCHU)-1)
C
C     WRITE LEGEND
C
      CALL TEXT_OUT('S'//char(0),(U0+UV/2.+LEGW)*SCALX,
     *              (V0+VV/2.-LABH)*SCALY,INT(10.*LCHU)-1)
      CALL TEXT_OUT('T'//char(0),(U0-UV/2.-LEGW)*SCALX,
     *              (V0+VV/2.-LABH)*SCALY,INT(10.*LCHU)-1)
      IF (NOTDS .EQ. 0) CALL TIMDS(MXU-15.*LCHU,MNV)
C
      CALL FLUSH_XWI
      RETURN
      END

      SUBROUTINE TPLOT(X,Y,NPOINTS,IFGRID,XAXIS,YAXIS,LABELX,NLX,
     1 LABELY,NLY,LABELZ,NLZ,KXTYPE,KYTYPE,KLINE,KSYMBL,KSTEPS,KSTEPL,
     2 SYSIZE,KXDEN,KYDEN,KXTICS,KYTICS,NOTIT,NOTDS)
C
      CHARACTER*(*)   LABELX, LABELY, LABELZ
      COMMON / TEKT / MINX,MAXX,MINY,MAXY,LCHX,LCHY,SCALX,SCALY
      REAL            MINX,MAXX,MINY,MAXY,LCHX,LCHY,SCALX,SCALY
      REAL            X(1),Y(1)
C
      IF (IFGRID .NE. 1) GO TO 8103
      CALL BINITT
      XUSE = MAXX-MINX-FLOAT(10+NLY)*LCHX
      YUSE = MAXY-MINY-6.*LCHY
      IF (XAXIS.GT.0. .AND. YAXIS.GT.0.) THEN
       IF (XAXIS*YUSE .GT. YAXIS*XUSE) THEN
        YUSE = XUSE*YAXIS/XAXIS
       ELSE
        XUSE = YUSE*XAXIS/YAXIS
       ENDIF
      ENDIF
      XSMIN = MINX+FLOAT(8+NLY)*LCHX
      XSMAX = XSMIN+XUSE
      YSMIN = MINY+3.*LCHY
      YSMAX = YSMIN+YUSE
      CALL SLIMX(XSMIN,XSMAX)
      CALL SLIMY(YSMIN,YSMAX)
      CALL NPTS(NPOINTS)
      IF (KXTYPE .NE. 1) CALL XTYPE(KXTYPE)
      IF (KYTYPE .NE. 1) CALL YTYPE(KYTYPE)
      IF (KXDEN.NE.0 .AND. KXDEN.NE.8) CALL XDEN(KXDEN)
      IF (KYDEN.NE.0 .AND. KYDEN.NE.8) CALL YDEN(KYDEN)
      IF (KXTICS .NE.0) CALL XTICS(KXTICS)
      IF (KYTICS .NE.0) CALL YTICS(KYTICS)
      CALL CHECK(X,Y)
      CALL LINE(KLINE)
      CALL SYMBL(KSYMBL)
      IF (KSTEPS.NE.0 .AND.  KSTEPS.NE.1)  CALL STEPS(KSTEPS)
      IF (KSTEPL.NE.0 .AND.  KSTEPL.NE.1)  CALL STEPL(KSTEPL)
      IF (SYSIZE.NE.0. .AND. SYSIZE.NE.1.) CALL SIZES(SYSIZE)
      CALL DSPLAY(X,Y)
      XPOS1 = MAX(0.,.5*(XSMIN+XSMAX)-FLOAT(NLX/2)*LCHX)
      XPOS2 = MAX(0.,.5*(XSMIN+XSMAX)-FLOAT(NLZ/2)*LCHX)
      YPOS  = .5 * (YSMIN + YSMAX)
      CALL TEXT_OUT(LABELX,XPOS1*SCALX,MINY*SCALY,
     *              INT(10.*LCHX)-1)
      CALL TEXT_OUT(LABELY,MINX*SCALX,YPOS*SCALY,
     *              INT(10.*LCHX)-1)
      IF (NOTIT .EQ. 0) CALL TEXT_OUT(LABELZ,XPOS2*SCALX,
     *                  (YSMAX+LCHY)*SCALY,INT(10.*LCHX)-1)
      IF (NOTDS .EQ. 0) CALL TIMDS(XSMAX-10.*LCHX,YSMAX-2.*LCHY)
      IF (IFGRID .EQ. 1) GO TO 8104
 8103 CONTINUE
      CALL NPTS(NPOINTS)
      CALL LINE(KLINE)
      CALL SYMBL(KSYMBL)
      IF (KSTEPS.NE.0 .AND. KSTEPS.NE.1)   CALL STEPS(KSTEPS)
      IF (KSTEPL.NE.0 .AND. KSTEPL.NE.1)   CALL STEPL(KSTEPL)
      IF (SYSIZE.NE.0. .AND. SYSIZE.NE.1.) CALL SIZES(SYSIZE)
      CALL CPLOT(X,Y)
 8104 CONTINUE
      CALL FLUSH_XWI
      RETURN
      END

      SUBROUTINE TRICT(X,Y,DX,DY,NOC,ZPLAN,ZX,ZV,ZY)
C
      COMMON /CNTRCOM/ ISYM(50)
      COMMON / CSYMB / NOSYM
      REAL             XP(2,50), YP(2,50), ZT(4), ZPLAN(1)
      CHARACTER*10     LABELX, LABELY, LABELZ
C
      ZT(1) = ZX
      ZT(2) = ZV
      ZT(3) = ZY
      ZT(4) = ZX
      ZTMIN = MIN(ZT(1),ZT(2),ZT(3))
      ZTMAX = MAX(ZT(1),ZT(2),ZT(3))
      IMIN  = NOC + 1
      IMAX  = 0
      DO 10 K = 1, NOC
      J = NOC - K + 1
      IF (ZPLAN(J) .GE. ZTMIN) IMIN = J
      IF (ZPLAN(K) .LE. ZTMAX) IMAX = K
   10 CONTINUE
      INT = IMAX - IMIN
      IF (INT.LT.0 .OR. ZTMIN.EQ.ZTMAX) GO TO 130
      I2 = 1
      DO 110 K = 1, 3
      ZTMAX = MAX(ZT(K),ZT(K+1))
      ZPMIN = MIN(ZT(K),ZT(K+1))
      MIN1  = NOC + 1
      MAX1  = 0
      DO 20 J = 1, NOC
      INZ = NOC-J+1
      IF (ZPLAN(INZ).GT.ZPMIN .OR. 
     1   (ZPLAN(INZ).EQ.ZPMIN .AND. ZTMIN.EQ.ZPMIN)) MIN1 = INZ
      IF (ZPLAN(J) .LE. ZTMAX) MAX1 = J
   20 CONTINUE
      INZ = MAX1 - MIN1
      IF (INZ.LT.0 .OR. ZTMAX.EQ.ZPMIN) GO TO 110
      IF (INZ-INT) 40,30,40
   30 GO TO (50,40), I2
   40 I2 = 1
      GO TO 60
   50 I2 = 2
   60 DO 100 J = MIN1, MAX1
      GO TO (70,80,90), K
   70 XP(I2,J) = X+DX*(ZPLAN(J)-ZT(2))/(ZT(1)-ZT(2))
      YP(I2,J) = Y
      GO TO 100
   80 XP(I2,J) = X
      YP(I2,J) = Y+DY*(ZPLAN(J)-ZT(2))/(ZT(3)-ZT(2))
      GO TO 100
   90 XP(I2,J) = X+DX*(ZPLAN(J)-ZT(3))/(ZT(1)-ZT(3))
      YP(I2,J) = Y+DY*(ZPLAN(J)-ZT(1))/(ZT(3)-ZT(1))
  100 CONTINUE
  110 CONTINUE
      DO 120 J = IMIN, IMAX
      ICOL = 2 + 3*(J-2*(J/2))	!	ROT/HELLBLAU
      CALL SET_COL_L(ICOL)	!	SWITCH ONLY LINE COLOR
      ISYM(J) = ISYM(J) + 1
      KSYMBL = 64 + J
      IF (NOSYM .EQ. 0) KSYMBL = 0
      IF (MOD(ISYM(J),11) .NE. 1) KSYMBL = 0
      IF (XP(1,J).EQ.X1OLD .AND. XP(2,J).EQ.X2OLD .AND.
     X    YP(1,J).EQ.Y1OLD .AND. YP(2,J).EQ.Y2OLD) GO TO 115
      IF (XP(1,J).EQ.XP(2,J) .AND. YP(1,J).EQ.YP(2,J)) GO TO 115
      CALL TPLOT(XP(1,J),YP(1,J),2,0,10.,10.,LABELX,NLX,
     X LABELY,NLY,LABELZ,NLZ,1,1,0,KSYMBL,3,1,SYSIZE
     X,0,0,0,0,0,0)
  115 X1OLD = XP(1,J)
      Y1OLD = YP(1,J)
      X2OLD = XP(2,J)
      Y2OLD = YP(2,J)
  120 CONTINUE
  130 RETURN
      END


      SUBROUTINE FRAME(N)
C
      COMMON / TEKT / MINX,MAXX,MINY,MAXY,LCHX,LCHY,SCALX,SCALY
      REAL            MINX,MAXX,MINY,MAXY,LCHX,LCHY,SCALX,SCALY
C
      REAL PARAM(6,10)
      DATA PARAM /0.0, 32.0,  0.5, 24.0, 0.320, 0.60,
     1            0.0,  9.0, 15.3, 22.8, 0.187, 0.35,
     2            0.0,  9.0,  7.7, 15.2, 0.187, 0.35, 
     3            0.0,  9.0,  0.1,  7.6, 0.187, 0.35, 
     4            9.5, 18.5, 15.3, 22.8, 0.187, 0.35, 
     5            9.5, 18.5,  7.7, 15.2, 0.187, 0.35, 
     6            9.5, 18.5,  0.1,  7.6, 0.187, 0.35, 
     7           18.5, 35.0, 12.0, 22.8, 0.187, 0.35, 
     8           18.7, 29.5,  6.0, 12.5, 0.187, 0.35, 
     9           19.5, 31.0,  0.1,  7.0, 0.187, 0.35/ 
C
      MINX = PARAM(1,N+1)
      MAXX = PARAM(2,N+1)
      MINY = PARAM(3,N+1)
      MAXY = PARAM(4,N+1)
      LCHX = PARAM(5,N+1)
      LCHY = PARAM(6,N+1)
C
      RETURN
      END

	SUBROUTINE MODXSCAL(XGRID,IM,JM,SOURCE)
C
C	FOR SUBROUTINE TCONTR
C
	REAL XGRID(51), SOURCE(51,51)
	REAL SAVEX(51), SAVES(51,51)
C
C	FIND ALL LOW XGRID VALUES WITH SOURCE-VALUES ZERO
C
	DO KK = 1, IM
	 SUM = 0.0
	 DO JJ = 1, JM
	  SUM = SUM + SOURCE(KK,JJ)
	 ENDDO
	 IF (SUM .NE. 0.0) GO TO 10
	ENDDO
10	I1 = MAX(1, KK - 1)
C
C	FIND ALL HIGH XGRID VALUES WITH SOURCE-VALUES ZERO
C
	DO KK = IM, 1, -1
	 SUM = 0.0
	 DO JJ = 1, JM
	  SUM = SUM + SOURCE(KK,JJ)
	 ENDDO
	 IF (SUM .NE. 0.0) GO TO 20
	ENDDO
20	I2 = MIN(51, KK + 1)
C
C	ONLY 2 POSSIBILITIES (FULL OR HALF FRAME) WANTED
C
	IF (I1.LT.13 .OR. I2.GT.38) THEN
	 I1 = 1		!	FULL FRAME
	 I2 = 51
	 RETURN
	ELSE
	 I1 = 13	!	HALF FRAME
	 I2 = 38
	ENDIF
C
C	SHRINK MATRIX (SAVE INITIAL VALUES)
C
	SAVEI = IM
	IM    = I2 - I1 + 1
	DO I  = 1, IM
	 SAVEX(I) = XGRID(I)
	 XGRID(I) = XGRID(I+I1-1)
	 DO J = 1, JM
	  SAVES(I,J)  = SOURCE(I,J)
	  SOURCE(I,J) = SOURCE(I+I1-1,J)
	 ENDDO
	ENDDO
	RETURN
C
	ENTRY RESXSCAL(XGRID,IM,JM,SOURCE)
C
C	RESTORE INITAL VALUES
C
	IF (IM .EQ. 51) RETURN
	DO I  = 1, IM
	 XGRID(I) = SAVEX(I)
	 DO J = 1, JM
	  SOURCE(I,J) = SAVES(I,J)
	 ENDDO
	ENDDO
	IM    = SAVEI
	RETURN
	END

	SUBROUTINE MODXSCAL2(XGRID,IM,JM,SOURCE)
C
C	FOR SUBROUTINE TISO3D
C
	REAL XGRID(51), SOURCE(51,51)
	REAL SAVEX(51), SAVES(51,51)
C
C	FIND ALL LOW XGRID VALUES WITH SOURCE-VALUES ZERO
C
	DO KK = 1, IM
	 SUM = 0.0
	 DO JJ = 1, JM
	  SUM = SUM + SOURCE(KK,JJ)
	 ENDDO
	 IF (SUM .NE. 0.0) GO TO 10
	ENDDO
10	I1 = MAX(1, KK - 1)
C
C	FIND ALL HIGH XGRID VALUES WITH SOURCE-VALUES ZERO
C
	DO KK = IM, 1, -1
	 SUM = 0.0
	 DO JJ = 1, JM
	  SUM = SUM + SOURCE(KK,JJ)
	 ENDDO
	 IF (SUM .NE. 0.0) GO TO 20
	ENDDO
20	I2 = MIN(51, KK + 1)
C
C	ONLY 2 POSSIBILITIES (FULL OR HALF FRAME) WANTED
C
	IF (I1.LT.13 .OR. I2.GT.38) THEN
	 I1 = 1		!	FULL FRAME
	 I2 = 51
	 RETURN
	ELSE
	 I1 = 13	!	HALF FRAME
	 I2 = 38
	ENDIF
C
C	SAVE INITIAL VALUES
C
	DO I  = 1, IM
	 SAVEX(I) = XGRID(I)
	 DO J = 1, JM
	  SAVES(I,J)  = SOURCE(I,J)
	 ENDDO
	ENDDO
C
C	FILL EVERY SECOND LINE (EXPANSION BY FACTOR OF 2)
C
	DO I = I1, I2
	 XGRID(2*(I-I1)+1) = XGRID(I)
	 DO J = 1, JM
	  SOURCE(2*(I-I1)+1,J) = SAVES(I,J)
	 ENDDO
	ENDDO
C
C	INTERPOLATE EVERY SECOND LINE
C
	DO I  = 1, IM-1, 2
	 XGRID(I+1) = (XGRID(I) + XGRID(I+2)) /2.
	 DO J = 1, JM
	  SOURCE(I+1,J) = (SOURCE(I,J) + SOURCE(I+2,J)) /2.
	 ENDDO
	ENDDO

	RETURN
C
	ENTRY RESXSCAL2(XGRID,IM,JM,SOURCE)
C
C	RESTORE INITAL VALUES
C
        IF (I2 .EQ. 51) RETURN
	DO I  = 1, IM
	 XGRID(I) = SAVEX(I)
	 DO J = 1, JM
	  SOURCE(I,J) = SAVES(I,J)
	 ENDDO
	ENDDO
	RETURN
	END

      SUBROUTINE MYTRIM(IN, OUT, N)
C
      CHARACTER *(*) IN, OUT
C
      DO 10 I = 1, N
       OUT(I:I) = IN(I:I)
  10  CONTINUE
C
      RETURN
      END








