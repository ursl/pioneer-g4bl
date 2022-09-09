c
c     Postscript subroutines created by Urs Rohrer (PSI)
c     Modified and improved 1985-1990. 
c
      SUBROUTINE PSOPEN(OUTPUT)
C
      CHARACTER*(*)   OUTPUT
      COMMON /COLTAG/ COLOR, XSCAL, YSCAL, XOFF, YOFF, CHSIZE, NDEV
      LOGICAL         COLOR
C
C     WRITE HEADER SO 'CUPS' RECOGNISES FILE AS POST-SCRIPT
C     (MUST START AT 1ST CHARACTER ON LINE)
C     (FORTRAN WRITES FIRST A BLANK !!!!)
C     (SO I DO IT IN C !!!!!)
C
      CHARACTER*20 FNAME
      FNAME = OUTPUT
      DO 10 J = 20, 1, -1
        IF (FNAME(J:J) .NE. ' ') GO TO 11
   10 CONTINUE
   11 FNAME(J+1:J+1) = CHAR(0)
      CALL INIPS(FNAME,'%!PS-Adobe-3.0'//char(0))
C
C     APPEND REST IN FORTRAN
C
      OPEN(UNIT=25,STATUS='UNKNOWN',ACCESS='APPEND',
     &     FILE=OUTPUT)
      WRITE(25,*) '/m {moveto} def'
      WRITE(25,*) '/d {lineto} def'
      WRITE(25,*) '/s {show} def'
      WRITE(25,*) '/w {setlinewidth} def'
      WRITE(25,*) '/st {stroke} def'
      WRITE(25,*) '/c {setrgbcolor} def'
      WRITE(25,*) 'newpath 1 setlinecap 1 setlinejoin'
      WRITE(25,*) '90 rotate'
      WRITE(25,*) '0 -533 translate'
      WRITE(25,*) '19 -23 translate'
      WRITE(25,1000) XOFF, YOFF
1000  FORMAT(2F6.1,' translate')
      RETURN
      END

      SUBROUTINE PSCLS
C
      WRITE(25,*) 'st showpage'
      CLOSE(UNIT=25)
      RETURN
      END

      SUBROUTINE PSMOV(X,Y)
C
      REAL X,Y
C
      COMMON /COLTAG/ COLOR, XSCAL, YSCAL, XOFF, YOFF, CHSIZE, NDEV
      LOGICAL         COLOR
C
      WRITE(25,1000,ERR=200) XSCAL * X, YSCAL * Y
1000  FORMAT(F7.2,F7.2,' m')
      RETURN
  200 WRITE(25,*) XSCAL,Y,YSCAL,Y
      RETURN
      END

      SUBROUTINE PSDRW(X,Y)
C
      REAL X,Y
C
      COMMON /COLTAG/ COLOR, XSCAL, YSCAL, XOFF, YOFF, CHSIZE, NDEV
      LOGICAL         COLOR
C
      WRITE(25,1000,ERR=200) XSCAL * X, YSCAL * Y
1000  FORMAT(F7.2,F7.2,' d')
      RETURN
  200 WRITE(25,*) XSCAL,Y,YSCAL,Y
      RETURN
      END

      SUBROUTINE PSTEXT(X,Y,TEXT)
C
      REAL X,Y
      CHARACTER*(*) TEXT
C
      COMMON /COLTAG/ COLOR, XSCAL, YSCAL, XOFF, YOFF, CHSIZE, NDEV
      LOGICAL         COLOR
C
      WRITE(25,1000) XSCAL * X, YSCAL * Y, TEXT
1000  FORMAT(F7.2,F7.2,' m (',A,') s st')
      RETURN
      END

      SUBROUTINE PSTXTS(SIZE)
C
      WRITE(25,1000) SIZE
1000  FORMAT(' /Courier findfont',F5.1,' scalefont setfont')
      RETURN
      END

      SUBROUTINE PSLINW(WIDTH)
C
      WRITE(25,1000) WIDTH
1000  FORMAT(' st',F5.2,' w st')
      RETURN
      END

      SUBROUTINE PSSETCOL(INDEX)
C
C     0 WEISS, 1 SCHWARZ, 2 ROT, 3 GRUEN, 4 BLAU(HELL)
C     5 ?, 6 VIOLETT, 7 GELB
C
      COMMON /COLTAG/ COLOR, XSCAL, YSCAL, XOFF, YOFF, CHSIZE, NDEV
      LOGICAL         COLOR
C
      REAL R(0:7), G(0:7), B(0:7)
      DATA R /1.,0.,1.,0.,0.,1.,0.,1./
      DATA G /1.,0.,0.,1.,1.,0.,0.,1./
      DATA B /1.,0.,0.,0.,1.,1.,1.,0./
C
      IF (.NOT. COLOR) RETURN
      WRITE(25,1000) R(INDEX),G(INDEX),B(INDEX)
1000  FORMAT(' st',3F4.1,' c st')
      RETURN
      END

      SUBROUTINE PSFLUSH
C
      WRITE(25,*) 'st'
      RETURN
      END

      SUBROUTINE PSLINEX(L)
C
C     Instead of dotted line thin line
C
      IF (L .EQ. 1) THEN
        CALL PSLINW(0.1)  !  thin
      ELSE
        CALL PSLINW(0.5)  !  normal
      ENDIF
C
      RETURN
      END

      SUBROUTINE PSNEWPAGE
C
      WRITE(25,*) 'showpage'
      WRITE(25,*) '/m {moveto} def'
      WRITE(25,*) '/d {lineto} def'
      WRITE(25,*) '/s {show} def'
      WRITE(25,*) '/w {setlinewidth} def'
      WRITE(25,*) '/st {stroke} def'
      WRITE(25,*) '/c {setrgbcolor} def'
      WRITE(25,*) 'newpath 1 setlinecap 1 setlinejoin'
      WRITE(25,*) '90 rotate'
      WRITE(25,*) '0 -533 translate'
      WRITE(25,*) '19 -23 translate'
      WRITE(25,1000) XOFF, YOFF
1000  FORMAT(2F6.1,' translate')
C
      RETURN
      END
