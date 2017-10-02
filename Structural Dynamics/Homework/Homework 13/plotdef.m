%----------------------------------------------------------------plotdef
    function plotdef(x,ix,id,u,stresses,d,...
                     numnod,numelm,numdim,jpos,sc,mtype)
%   *-----------------------------------------------------------------*
%   |                                                                 |
%   |   PLOTDEF: Plot deflected shape                                 |
%   |                                                                 |
%   |   Incoming values:                                              |
%   |      x = array nodal coordinates for ends of the element        |
%   |      ix = array of element connectivities                       |
%   |      id = array of equation numbers                             |
%   |      u  = array of global displacements                         |
%   |      stresses = array of element stresses                       |
%   |      d = array of element properties                            |
%   |      numnod = number of nodes in structure                      |
%   |      numelm = number of elements in the structure               |
%   |      numdim = dimension of geometric space (i.e., 2D or 3D)     |
%   |      jpos   = number of unrestrained degrees of freedom         |
%   |      sc     = displacement magnification for plotting           |
%   |      type   = options for line type in plotgeo                  |
%   |                = 0 no plot                                      |
%   |                = 1 dotted lines red                             |
%   |                = 2 solid lines blue                             |
%   |                = 3 colormap for stress ratio                    |
%   |                = 4 compression(green), low(blue), tension(red)  |
%   |                                                                 |
%   |   Outgoing values:                                              |
%   |      The output is a graphic                                    |
%   |                                                                 |
%   *-----------------------------------------------------------------*
      
%.... Compute node locations after deformation
      x1 = x;         
      for i=1:numnod
        for j=1:numdim
            ii = id(i,j); 
            if (ii < 0)
              ii = jpos - ii; 
            end
            x1(i,j) = x(i,j) + sc*u(ii);
        end
      end
    
%.... Plot deflected shape
      plotgeo(x1,ix,stresses,d,numelm,numdim,mtype);
      
      return