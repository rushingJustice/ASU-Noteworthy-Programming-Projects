%------------------------------------------------------------------plotgeo
    function plotgeo(x,ix,stresses,d,numelm,numdim,type)
%   *--------------------------------------------------------------------*
%   |                                                                    |
%   |   PLOTGEO: Plot problem geometry                                   |
%   |                                                                    |
%   |   Incoming values:                                                 |
%   |      x = array nodal coordinates for ends of the element           |
%   |      ix = array of element connectivities                          |
%   |      stresses = array of element stresses                          |
%   |      d = array of element properties                               |
%   |      numnod = number of nodes in structure                         |
%   |      numelm = number of elements in the structure                  |
%   |      numdim = dimension of geometric space (i.e., 2D or 3D)        |
%   |      lineformat   = style of line for plotting                     |
%   |                                                                    |
%   |   Outgoing values:                                                 |
%   |      The output is a graphic                                       |
%   |                                                                    |
%   *--------------------------------------------------------------------*

%.... Establish color map for plotting stresses
      cm = colormap(jet);
      size = 64;
      gr = 0.33;
      
%.... Establish marker size (ms) and line weight (wt)      
      ms = 2 + ceil(160/numelm);
      wt = 1 + 20/numelm;
      lineformat = 'o-'; 
      mc = 'k';
      c  = 'k';

%.... Set the line type, weight, and color depending on type
      if (type==1)
        lineformat = 'o-'; 
        mc = 'b';
        c  = 'b';
      elseif (type==2)
        lineformat = 'o:';
        mc = 'r';
        c  = 'r';
      elseif (type==3)
        lineformat = 'o-';
        wt = 2 + 20/numelm;
        ms = 1;
        mc = 'k';
      elseif (type==4)
        lineformat = 'o-';
        wt = 2 + 20/numelm;
        size = 3;
        ms = 1;
        mc = 'k';
      end

      if (type > 0)
          
%....... Loop over all elements in the structure   
        for n=1:numelm
        
%........ Find the i-node, j-node for element n          
          inode = ix(n,1);
          jnode = ix(n,2); 
          mat   = ix(n,3);

%........ Find coordinates of nodes connected to element n
          xe(1,:) = x(inode,:);
          xe(2,:) = x(jnode,:);
          No = d(mat,2);
       
%........ Assign member color in accord with stress ratio  
          if (type==3)
            sr = stresses(n,2)/No;
            mp = floor((size + 1 +(size-1)*sr)/2);
            c = cm(mp,:);
          end
          
%........ Assign member color in accord with stress ratio
          if (type==4)
            sr = stresses(n,2)/No;
            if (sr < -gr)
                c = 'g';
            elseif (sr > gr)
                c = 'r';
            else
                c = 'b';
            end
          end
            
%........ Plot element
          if (numdim==2)
              plot(xe(:,1),xe(:,2),lineformat,...
                                  'LineWidth',wt,'Color',c,...
             'MarkerEdgeColor',mc,'MarkerFaceColor',mc,'MarkerSize',ms); 
          else
              plot3(xe(:,1),xe(:,2),xe(:,3),lineformat,...
                                'LineWidth',wt,'Color',c,...
             'MarkerEdgeColor',mc,'MarkerFaceColor',mc,'MarkerSize',ms); 
          end
          
        end  % loop over elements 
        
      end % if type==0
      
      return