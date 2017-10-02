%----------------------------------------------------------------plotdef
    function plotdef(x,ix,id,u,numnod,numelm,numdim,jpos)
%   *-----------------------------------------------------------------*
%   |                                                                 |
%   |   PLOTDEF: Plot deflected shape                                 |
%   |                                                                 |
%   |   Incoming values:                                              |
%   |      x = array nodal coordinates for ends of the element        |
%   |      ix = array of element connectivities                       |
%   |      id = array of equation numbers                             |
%   |      numnod = number of nodes in structure                      |
%   |      numelm = number of elements in the structure               |
%   |      numdim = dimension of geometric space (i.e., 2D or 3D)     |
%   |      jpos   = number of unrestrained degrees of freedom         |
%   |                                                                 |
%   |   Outgoing values:                                              |
%   |      The output is a graphic                                    |
%   |                                                                 |
%   *-----------------------------------------------------------------*
    
%.... Displacement magnifying scale
      sc = 25;         % Displacement magnification factor for plot
    
%.... Compute node locations after deformation
      x1 = x;         % Initialize just to fix dimensions
      for i=1:numnod
        for j=1:numdim
            ii = id(i,j); if ii<0, ii = jpos - ii; end
            x1(i,j) = x(i,j) + sc*u(ii);
        end
      end
    
%.... Plot deflected shape
      fig=figure(1); clf; grid on; hold on;
      plotgeo(x ,ix,numelm,numdim,0);
      plotgeo(x1,ix,numelm,numdim,1);
      title(' -o initial geometry       .. deflected shape')