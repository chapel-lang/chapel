import math

'''

A very, very basic & crude implementation of the tensor/matrix/vector
classes used by MADNESS.  This to enable a standalone and pure Python
version of the 1d example code.

'''

class Vector:
    def __init__(self,arg,wrap=0):
        '''
        If arg is an integer, make a new zero vector of length n.
        Otherwise, arg must be a python sequence or another vector,
        and a new (deep) copy is generated unless wrap is true in
        which case the existing sequence is wrapped.

        Inheritance from list does not work as desired so we have to wrap it.
        '''
        if isinstance(arg,int):
            self.a = [0.0]*arg
        else:
            self.a = list(arg)

    def normf(self):
        sum = 0.0
        for i in range(len(self.a)):
            sum += self.a[i] * self.a[i]
        return math.sqrt(sum)

    def inner(self,other):
        sum = 0.0
        for i in range(len(self.a)):
            sum += self.a[i] * other.a[i]
        return sum

    def gaxpy(self,alpha,other,beta):
        '''
        generalized saxpy (inplace) ... self = self*alpha + other*beta
        '''
        if not isinstance(other,Vector):
            raise TypeError,"you're adding a vector to what???"
        for i in range(len(self.a)):
            self.a[i] = alpha*self.a[i] + beta*other.a[i]
        return self

    def scale(self,s):
        for i in range(len(self.a)):
            self.a[i] *= s
        return self

    def emul(self, other):
        for i in range(len(self.a)):
            self.a[i] *= other.a[i]
        return self

    def __getitem__(self,ind):
        return self.a[ind]

    def __setitem__(self,ind,value):
        self.a[ind] = value

    def __getslice__(self,lo,hi):
        return Vector(self.a[lo:hi],wrap=1)

    def __setslice__(self,lo,hi,value):
        self.a[lo:hi] = value

    def __str__(self):
        return str(self.a)

    def __len__(self):
        return len(self.a)

    def __add__(self,other):
        r = Vector(self)
        return r.gaxpy(1.0,other,1.0)

class Matrix:
    def __init__(self,arg0,arg1=None,wrap=0):
        '''
        If arg0,arg1 is a pair of integers (n,m) make a zero n*m matrix.

        If arg0 is another Matrix make a new (deep) copy unless wrap is
        true, in which case just wrap the argument (i.e., data is
        shared).
        '''
        if isinstance(arg0,Matrix):
            if wrap:
                self.a = arg.a
            else:
                n,m = arg.dims()
                self.a = [0.0]*n
                for i in range(n):
                    self.a[i] = Vector(m)
                    for j in range(m):
                        self.a[i][j] = arg.a[i][j]
        elif isinstance(arg0,int) and isinstance(arg1,int):
            n,m = arg0,arg1
            self.a = [0.0]*n
            for i in range(n):
                self.a[i] = Vector(m)
        else:
            raise ValueError,"invalid argument to matrix constructor"

    def dims(self):
        return len(self.a),len(self.a[0])

    def __getitem__(self,ind):
        '''
        ind is a pair of integers -> returns an element
        '''
        i,j = ind
        return self.a[i][j]

    def __setitem__(self,ind,value):
        '''
        ind is a pair of integers -> set the element
        '''
        i,j = ind
        self.a[i][j] = value

    def __str__(self):
        '''
        convert matrix to string for simple printing
        '''
        r = ''
        n,m = self.dims()
        for i in range(n):
            for j in range(m):
                r += ' %12.6f' % self.a[i][j]
            r += '\n'
        r += '\n'
        return r

    def __mul__(self,v):
        '''
        matrix * column vector multiplication returning a new vector
        '''
        if not isinstance(v,Vector):
            raise TypeError,"matrix*vector expected a vector"

        n,m = self.dims()

        if m != len(v):
            raise ValueError,"matrix and vector do not conform"

        r = Vector(n)
        for i in range(n):
            for j in range(m):
                r[i] += self.a[i][j] * v[j]

        return r

    def __rmul__(self,v):
        '''
        row vector * matrix returning a new vector
        '''
        if not isinstance(v,Vector):
            raise TypeError,"matrix*vector expected a vector"

        n,m = self.dims()

        if n != len(v):
            raise ValueError,"matrix and vector do not conform"

        r = Vector(m)
        for i in range(m):
            for j in range(n):
                r[i] += v[j] * self.a[j][i] 

        return r


if __name__ == "__main__":

    # Test getting/setting elements of a matrix
    a = Matrix(3,4)
    for i in range(3):
        for j in range(4):
            a[i,j] = i*100 + j

    print a

    for i in range(3):
        for j in range(4):
            if a[i,j] != i*100 + j:
                raise ValueError,"incorrect value at (%d,%d)" % (i,j)

    v = Vector([1.1,2.02,3.003,4.004])

    # Test matrix*vector and vector*matrix products
    av = a*v
    for i in range(3):
        sum = 0.0
        for j in range(4):
            sum += a[i,j]*v[j]
        if (sum-av[i]) != 0.0:
            print sum,av[i]
            raise ValueError,"incorrect av at %d" % i

    v = Vector([1.1,2.02,3.003])
    va = v*a
    for i in range(4):
        sum = 0.0
        for j in range(3):
            sum += v[j]*a[j,i]
        if sum-va[i] != 0.0:
            raise ValueError,"incorrect va at %d" % i


    print "there is a remote chance all is OK"

