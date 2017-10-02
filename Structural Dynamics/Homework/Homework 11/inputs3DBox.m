%------------------------------------------------------------inputs3D
      function [numdim,x,ix,id,d,f,numnod,numelm,nummat]=inputs3D
%   *-----------------------------------------------------------*
%   | Read problem data:                                        |
%   |   numdim  : Number of dimensions                          |
%   |             =2 for 2D structures and 3 for 3D structures  |
%   |   numnod  : Number of Nodes in the structure              |
%   |   numelm  : Number of Elements in the structure           |
%   |   nummat  : Number of Material Sets in the structure      |
%   |   x (n,3) : X,Y, Z coordinates            n=1,...,numnod  |
%   |   ix(m,3) : I-node, J-node, Material set  m=1,...,numelm  |
%   |   id(n,3) : BC1, BC2, BC3                 n=1,...,numnod  |
%   |   d (k,1) : EA,                           k=1,...,nummat  |
%   |   f (n,3) : X-force, Y-force, Z-force     n=1,...,numnod  |
%   *-----------------------------------------------------------*
        
%...Problem dimensions
    numdim = 3;
    numnod = 12;
    numelm = 40;
    nummat = 5;
    
%...Nodal coordinates, each row is (x, y, z) in nodal order
yscale = 5;
xscale = 5;

y = yscale;
x = xscale;

x = [  0.00*x    0.00    0.0 ;
       5.00*x    0.00    0.0 ;
      10.00*x     0.00    0.0 ;
       0.00*x     0.00    5.0 ;
       5.00*x     0.00    5.0 ;
      10.00*x     0.00    5.0 ;
       0.0*x      y    0.0;
       5.0*x      y    0.0;
      10.0*x      y    0.0;
       0.0*x      y    5.0;
       5.0*x      y    5.0;
      10.0*x      y    5.0] ; 
      
      

%...Element Connectivity, each row is (i-node, j-node, matl set) in element order
ix = [  1    2    1 ;
        2    3    1 ;
        1    4    2 ;
        4    5    3 ;
        5    6    3 ;
        6    3    2 ;
        6    2    2 ;
        2    4    2 ;
        2    5    2 ;
        7    8    1 ;
        8    9    1 ;
        7    10   2;
        10   11   3;
        11   12   3;
        9    12   2;
        12   8    2;
        8    11   2;
        8    10   2;
        1    7    4;
        7    2    4;
        1    8    4;
        2    8    4;
        2    9    4;
        8    3    4;
        3    9    4;
        4    10   5;
        4    11   5;
        10   5    5;
        5    11   5;
        11   6    5;
        5    12   5;
        12   6    5 ;
        1    5    2;
        5    3    2;
        7    11   2;
        11   9    2;
        1    10   2;
        7    4    2;
        3    12   2;
        6    9    2];
         

%.... Material Set, each row is (EA) in material set order
d = [ 5000.0, 2*10^10 ; 
      5000.0, 2*10^10 ;
      5000.0, 2*10^10 ;
      5000.0, 2*10^10 ;
      5000.0, 2*10^10 ];
  
%.... Boundary Condition, each row is (x-dof, y-dof, z-dof: 1=fixed, 0=free)
id = [  1    1    1 ;
        0    0    0 ;
        1    1    1 ;
		0    0    0 ;
        0    0    0 ;
        0    0    0 ;
        1    1    1 ;
        0    0    0 ;
        1    1    1 ;
        0    0    0 ;
        0    0    0 ;
        0    0    0 ];
       

%.... Nodal force, each row is (x-force, y-force, z-force) in node order
f = [   0    0    0 ;
        0    1    0 ;
        0    0    0 ;
		0    0    0 ;
        0    1    0 ;
        0    0    0 ;
        0    0    0 ;
        0    0    0 ;
        0    0    0 ;
        0    0    0 ;
        0    0    0 ;
        0    0    0 ];
 

%.... Check to see if problem data are consistent
      [xsize,tstdim]= size(x); 
      [ixsize,null] = size(ix); 
      [dsize,null]  = size(d); 
      [idsize,null] = size(id); 
      [fsize,null]  = size(f); 
      
      if xsize~=numnod  error('Number of nodes in X is not equal to NUMNOD'),     end
      if tstdim~=numdim error('Number of cols in X is not equal to NUMDIM'),     end
      if ixsize~=numelm error('Number of elements in IX is not equal to NUMELM'), end
      if dsize~=nummat  error('Number of materials in D is not equal to NUMMAT'), end
      if idsize~=numnod error('Number of boundary conditions in ID not correct'), end
      if fsize~=numnod  error('Number of forces input in F is not correct'),      end
          
   return