%----------------------------------------------------------------plotgeo
    function plotgeo(x,ix,numelm,numdim,type)
%   *-----------------------------------------------------------------*
%   |                                                                 |
%   |   PLOTGEO: Plot problem geometry                                |
%   |                                                                 |
%   |   Incoming values:                                              |
%   |      x = array nodal coordinates for ends of the element        |
%   |      ix = array of element connectivities                       |
%   |      numnod = number of nodes in structure                      |
%   |      numelm = number of elements in the structure               |
%   |      numdim = dimension of geometric space (i.e., 2D or 3D)     |
%   |      lineformat   = style of line for plotting                  |
%   |                                                                 |
%   |   Outgoing values:                                              |
%   |      The output is a graphic                                    |
%   |                                                                 |
%   *-----------------------------------------------------------------*

%.... Set the line type, weight, and color depending on type
      if (type==0)
        lineformat = 'o-'; 
        mc = 'b';
        ms = 7;
        wt = 2;
        c  = 'b';
      else
        lineformat = 'o:';
        mc = 'r';
        ms = 5;
        wt = 1;
        c  = 'r';
      end

%.... Loop over all elements in the structure   
      for n=1:numelm
        
%...... Find the i-node, j-node for element n          
        inode = ix(n,1);
        jnode = ix(n,2);

%...... Find coordinates of nodes connected to element n
        xe(1,:) = x(inode,:);
        xe(2,:) = x(jnode,:);

%...... Plot element
        if (numdim==2)
            plot(xe(:,1),xe(:,2),lineformat,...
                                'LineWidth',wt,'Color',c,...
                                'MarkerFaceColor',mc,'MarkerSize',ms); 
        else
            plot3(xe(:,1),xe(:,2),xe(:,3),lineformat,...
                                'LineWidth',wt,'Color',c,...
                                'MarkerFaceColor',mc,'MarkerSize',ms); 
        end
      end
      xlabel('x');ylabel('y');zlabel('z');
      axis equal; view(numdim);
      
      return