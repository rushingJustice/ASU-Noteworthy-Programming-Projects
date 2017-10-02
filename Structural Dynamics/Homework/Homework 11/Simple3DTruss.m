%------------------------------------------------------------inputs3D
      function [numdim,x,ix,id,d,f,numnod,numelm,nummat]=Simple3DTruss
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
    numnod = 4;
    numelm = 3;
    nummat = 1;
    
%...Nodal coordinates, each row is (x, y, z) in nodal order
x = [  72.00    0.00    0.0 ;
       72.00  108.00    0.0 ;
        0.00  108.00   36.0 ;
        0.00    0.00   84.0 ];

%...Element Connectivity, each row is (i-node, j-node, matl set) in element order
ix = [  1    2    1 ;
        3    2    1 ;
        4    2    1 ];

%.... Material Set, each row is (EA) in material set order
%     d = [EA, Yield]
d = [ 29000 , 10000000.0];
  
%.... Boundary Condition, each row is (x-dof, y-dof, z-dof: 1=fixed, 0=free)
id = [  1    1    1 ;
        0    0    0 ;
        1    1    1 ;
		1    1    1 ];

%.... Nodal force, each row is (x-force, y-force, z-force) in node order
f = [  0.0    0.0      0.0 ;
       0.0    0.0      0.0 ;
       0.0    0.0      0.0 ;
       0.0    0.0      0.0 ];

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