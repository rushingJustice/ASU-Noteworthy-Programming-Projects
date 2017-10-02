%--------------------------------------------------------------getplotbox
      function [bounds] = getplotbox(x,numdim)
%   *--------------------------------------------------------------------*
%   |                                                                    |
%   |   GETPLOTBOX: Compute the size of the box for the axis command     |
%   |                                                                    |
%   |   Incoming values:                                                 |
%   |      x = array nodal coordinates for ends of the element           |
%   |      numdim = dimension of space                                   |
%   |                                                                    |
%   |   Outgoing values:                                                 |
%   |      bounds = [xmin xmax ymin ymax ]           for numdim = 2      |
%   |      bounds = [xmin xmax ymin ymax zmin zmax ] for numdim = 3      |
%   |                                                                    |
%   *--------------------------------------------------------------------*
      
%.... Establish the boundary padding for the region 
      borderx = 0.1;
      bordery = 0.4; 
      borderz = 0.1; 
      
%.... Establish the largest and smallest of the x, y, z values of coords      
      smallestx = min(x(:,1)); greatestx = max(x(:,1));
      dx = (greatestx - smallestx)*borderx;
      xmin = smallestx - dx; xmax = greatestx + dx;
      
      smallesty = min(x(:,2)); greatesty = max(x(:,2));
      dy = (greatesty - smallesty)*bordery;
      ymin = smallesty - dy; ymax = greatesty + dy;
      
      if (numdim==3)
        smallestz = min(x(:,3)); greatestz = max(x(:,3));
        dz = (greatestz - smallestz)*borderz;
        zmin = smallestz - dz; zmax = greatestz + dz;
      end
      
%.... Set the array with bounds ready for axis command      
      if (numdim==2)
        bounds = [xmin xmax ymin ymax];
      elseif (numdim==3)
        bounds = [xmin xmax ymin ymax zmin zmax ]; 
      end
      
      return