      program main
      implicit none
c
      integer n ! no. of basis functions
      integer natom ! no. of atoms
c
c  For this simple example code we will put 10 basis functions
c  on even numbered atoms and 5 basis functions on odd numbered
c  atoms.  In practice, it will not be as regular as this
c  and would be driven off the array bas_info (made below).
c
      parameter (natom=5)
      parameter (n=(natom/2)*10 + ((natom+1)/2)*5)
c
c  In practice, d, k, and j would be distributed
c  by blocks so that the range of indices associated
c  with each atom would be co-located on a processor.
c
      double precision dmat(n,n) ! density matrix
      double precision kmat(n,n) ! exchange matrix
      double precision jmat(n,n) ! coulomb matrix
      double precision kmat1(n,n) ! exchange matrix
      double precision jmat1(n,n) ! coulomb matrix
      double precision kmat2(n,n) ! exchange matrix
      double precision jmat2(n,n) ! coulomb matrix
c
      integer bas_info(2,natom) ! (1,atom)-> first bf on atom 
                                ! (2,atom)-> last bf on atom
c
      integer atom, i, j, nbf, nbf_atom
c
c  Construct the range of basis functions on each atom
c  according to the simple recipe given above
c
      nbf = 0
      do atom = 1, natom
         nbf_atom = 10/(mod(atom,2)+1)
         bas_info(1,atom) = nbf+1
         bas_info(2,atom) = nbf+nbf_atom
         nbf = nbf + nbf_atom
         write(6,*) ' atom ', atom, bas_info(1,atom), bas_info(2,atom)
      end do
      if (nbf .ne. n) stop 'robert cannot count'
c
c     Initialize the matrices.  D is the only input matrix.  J, K are
c     the output matrices and are zero initialized so that they can be
c     accumulated into.
c
      do i = 1, n
         do j = 1, n
            dmat(i,j) = 1.0d0/(i+j)
            jmat(i,j) = 0.0d0
            kmat(i,j) = 0.0d0
            jmat1(i,j) = 0.0d0
            kmat1(i,j) = 0.0d0
            jmat2(i,j) = 0.0d0
            kmat2(i,j) = 0.0d0
         end do
      end do
c
      call buildjk_very_simple(n, dmat, jmat, kmat)
      call buildjk_simple(n, dmat, jmat1, kmat1)
      call buildjk(natom, n, bas_info, dmat, jmat2, kmat2)
c
      write(6,*) ' first column of J & K for correctness check'
      do i = 1, n
         write(6,1) i, jmat(1,i), jmat1(1,i), jmat2(1,i), 
     $        kmat(1,i), kmat1(1,i), kmat2(1,i)
 1       format(i4,6f12.6)
      end do
c
      end
      subroutine buildjk_very_simple(n, dmat, jmat, kmat)
      implicit none
c
c     Unblocked loop version without using index symmetry (8x slower)
c
      integer n
      double precision dmat(n,n), jmat(n,n), kmat(n,n)
c
      integer i, j, k, l
      double precision g   ! function that evaluates the integrals
      external g
      double precision gijkl
c
      do i = 1, n
         do j = 1, n
            do k = 1, n
               do l = 1, n
                  gijkl = g(i,j,k,l)
                  jmat(i,j) = jmat(i,j) + dmat(k,l)*gijkl
                  kmat(i,k) = kmat(i,k) + dmat(j,l)*gijkl
               end do
            end do
         end do
      end do
c
      end
      subroutine buildjk_simple(n, dmat, jmat, kmat)
      implicit none
c
      integer n
      double precision dmat(n,n), jmat(n,n), kmat(n,n)
c
c     Unblocked loop version using index symmetries
c
      integer i, j, k, l, ltop
      double precision g   ! function that evaluates the integrals
      external g, scale
      double precision gijkl, facij, facijkl
c
      do i = 1, n
         do j = 1, i
            facij = 1.0
            if (i.eq.j) facij = facij*0.5
            do k = 1, i
               ltop = k
               if (k .eq. i) ltop = j
               do l = 1, ltop
                  facijkl = facij
                  if (k.eq.l) facijkl = facijkl*0.5d0
                  if (i.eq.k .and. j.eq.l) facijkl = facijkl*0.5d0
                  gijkl = g(i,j,k,l)*facijkl
                  jmat(i,j) = jmat(i,j) + dmat(k,l)*gijkl
                  jmat(k,l) = jmat(k,l) + dmat(i,j)*gijkl
                  kmat(i,k) = kmat(i,k) + dmat(j,l)*gijkl
                  kmat(i,l) = kmat(i,l) + dmat(j,k)*gijkl
                  kmat(j,k) = kmat(j,k) + dmat(i,l)*gijkl
                  kmat(j,l) = kmat(j,l) + dmat(i,k)*gijkl
               end do
            end do
         end do
      end do
c
      call symmetrize(jmat, n)
      call symmetrize(kmat, n)
      call scale(jmat, n, 4.0d0)
      call scale(kmat, n, 2.0d0)
c
      end
      subroutine buildjk(natom, n, bas_info, dmat, jmat, kmat)
      implicit none
c     
      integer natom, n
      integer bas_info(2,natom)
      double precision dmat(n,n), jmat(n,n), kmat(n,n)
c     
      integer iat, jat, kat, lat, lattop
c     
c     local copies of the atomic blocks of the density matrices
c     
      double precision dij(100), dik(100), dil(100), 
     $     djk(100), djl(100), dkl(100)
      double precision jij(100), jkl(100), kik(100), kil(100), 
     $     kjk(100), kjl(100)
c     
      integer ilo, ihi, jlo, jhi, klo, khi, llo, lhi ! range on each atom
      external scale
c     
c     This entire nest of loops can run in parallel
c
      do iat = 1, natom
         ilo = bas_info(1,iat)
         ihi = bas_info(2,iat)
         do jat = 1, iat
            jlo = bas_info(1,jat)
            jhi = bas_info(2,jat)
            do kat = 1, iat
               klo = bas_info(1,kat)
               khi = bas_info(2,kat)
               lattop = kat
               if (kat .eq. iat) lattop = jat
               do lat = 1, lattop
                  llo = bas_info(1,lat)
                  lhi = bas_info(2,lat)
c     
                  call buildjk_atom4(
     $                 dmat, jmat, kmat, n,
     $                 ilo, ihi, jlo, jhi, klo, khi, llo, lhi,
     $                 dij, dik, dil, djk, djl, dkl,
     $                 jij, jkl, kik, kil, kjk, kjl)
c     
               end do
            end do
         end do
      end do
c
      call symmetrize(jmat, n)
      call symmetrize(kmat, n)
      call scale(jmat, n, 4.0d0)
      call scale(kmat, n, 2.0d0)
c     
      end
      subroutine buildjk_atom4(
     $     dmat, jmat, kmat, n, 
     $     ilo, ihi, jlo, jhi, 
     $     klo, khi, llo, lhi,
     $     dij, dik, dil, djk, djl, dkl,
     $     jij, jkl, kik, kil, kjk, kjl)
      implicit none
c     
      integer n
      double precision dmat(n,n), jmat(n,n), kmat(n,n)
      integer ilo, ihi, jlo, jhi, klo, khi, llo, lhi
c     
      double precision dij(ilo:ihi,jlo:jhi) 
      double precision dik(ilo:ihi,klo:khi) 
      double precision dil(ilo:ihi,llo:lhi) 
      double precision djk(jlo:jhi,klo:khi) 
      double precision djl(jlo:jhi,llo:lhi) 
      double precision dkl(klo:khi,llo:lhi) 
      double precision jij(ilo:ihi,jlo:jhi) 
      double precision jkl(klo:khi,llo:lhi) 
      double precision kik(ilo:ihi,klo:khi) 
      double precision kil(ilo:ihi,llo:lhi) 
      double precision kjk(jlo:jhi,klo:khi) 
      double precision kjl(jlo:jhi,llo:lhi) 
c     
      integer i, j, k, l, jtop, ktop, ltop
      logical oij, okl, oikjl
      double precision g
      external g
      double precision gijkl, facij, facijkl
c
      oij = ilo .eq. jlo
      okl = klo .eq. llo
      oikjl = ilo.eq.klo .and. jlo.eq.llo
c
      call get(dmat, n, ilo, ihi, jlo, jhi, dij)
      call get(dmat, n, ilo, ihi, klo, khi, dik)
      call get(dmat, n, ilo, ihi, llo, lhi, dil)
      call get(dmat, n, jlo, jhi, klo, khi, djk)
      call get(dmat, n, jlo, jhi, llo, lhi, djl)
      call get(dmat, n, klo, khi, llo, lhi, dkl)
c     
      call zero(jij, ilo, ihi, jlo, jhi)
      call zero(jkl, klo, khi, llo, lhi)
      call zero(kik, ilo, ihi, klo, khi)
      call zero(kil, ilo, ihi, llo, lhi)
      call zero(kjk, jlo, jhi, klo, khi)
      call zero(kjl, jlo, jhi, llo, lhi)
c     
      do i = ilo, ihi
         jtop = jhi
         if (oij) jtop = i
         do j = jlo, jtop
            facij = 1.0
            if (i .eq. j) facij = 0.5d0
            ktop = khi
            if (oikjl) ktop = i
            do k = klo, ktop
               ltop = lhi
               if (okl) ltop = k
               if (oikjl .and. k.eq.i) ltop = j
               do l = llo, ltop
                  facijkl = facij
                  if (k .eq. l) facijkl = facijkl*0.5d0
                  if (i.eq.k .and. j.eq.l) facijkl = facijkl*0.5d0
                  gijkl = g(i,j,k,l)*facijkl
                  jij(i,j) = jij(i,j) + dkl(k,l)*gijkl
                  jkl(k,l) = jkl(k,l) + dij(i,j)*gijkl
                  kik(i,k) = kik(i,k) + djl(j,l)*gijkl
                  kil(i,l) = kil(i,l) + djk(j,k)*gijkl
                  kjk(j,k) = kjk(j,k) + dil(i,l)*gijkl
                  kjl(j,l) = kjl(j,l) + dik(i,k)*gijkl
               end do
            end do
         end do
      end do
c
      call acc(jmat, n, ilo, ihi, jlo, jhi, jij)
      call acc(jmat, n, klo, khi, llo, lhi, jkl)
      call acc(kmat, n, ilo, ihi, klo, khi, kik)
      call acc(kmat, n, ilo, ihi, llo, lhi, kil)
      call acc(kmat, n, jlo, jhi, klo, khi, kjk)
      call acc(kmat, n, jlo, jhi, llo, lhi, kjl)
c
      end
      double precision function g(i,j,k,l) 
      implicit none
c
      integer i, j, k, l
c
      g = 1.0d0/(i*j+k*l)
      return
c
      end
      subroutine zero(f,ilo,ihi,jlo,jhi)
      implicit none
c
      integer ilo, ihi, jlo, jhi
      double precision f(ilo:ihi, jlo:jhi)
c
      integer i, j
c
      do i = ilo, ihi
         do j = jlo, jhi
            f(i,j) = 0.0d0
         end do
      end do
c
      end
      subroutine get(global, n, ilo, ihi, jlo, jhi, local)
      implicit none
c
c     Get a rectangular patch from a global array
c
      integer n, ilo, ihi, jlo, jhi
      double precision global(n,n), local(ilo:ihi, jlo:jhi)
c
      integer i, j
c
      do i = ilo, ihi
         do j = jlo, jhi
            local(i,j) = global(i,j)
         end do
      end do
c
      end
      subroutine acc(global, n, ilo, ihi, jlo, jhi, local)
      implicit none
c
c     Accumulate a rectangular patch into a global array
c
      integer n, ilo, ihi, jlo, jhi
      double precision global(n,n), local(ilo:ihi, jlo:jhi)
c
      integer i, j
c
      do i = ilo, ihi
         do j = jlo, jhi
            global(i,j) = global(i,j) + local(i,j)
         end do
      end do
c
      end
      subroutine symmetrize(f, n)
      implicit none
c
c     Symmetrize a global array
c
      integer n
      double precision f(n,n)
c
      integer i, j
c
      do i = 1, n
         do j = 1, i
            f(i,j) = (f(i,j) + f(j,i))*0.5d0
            f(j,i) = f(i,j)
         end do
      end do
c
      end
      subroutine scale(f, n, s)
      implicit none
c
c     Scale a global array
c
      integer n
      double precision f(n,n), s
c
      integer i, j
c
      do i = 1, n
         do j = 1, n
            f(i,j) = f(i,j)*s
         end do
      end do
c
      end
