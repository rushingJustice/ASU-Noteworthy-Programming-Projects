%   *-------------------------------------------------------------------*
%   |  NUMERICAL INTEGRATION FREE VIBRATION                             |  
%   |    Central Difference Method                                      |                                                              |
%   |    This program solves the benchmark free vibration problems for  |
%   |    the central difference method to examine its accuracy and      |
%   |    stability characteristics.                                     |
%   |                                                                   |                                                  
%   |  Author:  K. D. Hjelmstad                                         |
%   |  Original Version:  01.22.2013                                    |
%   |  Last Modification: 02.04.2014                                    |
%   |                                                                   |
%   |  Associated Files:                                                |
%   |     none                                                          |
%   *-------------------------------------------------------------------*

      clear; clc;
      
%.... Print Banner to command window
      fprintf('%s\n','*------------------------------------------------*')
      fprintf('%s\n','|        NUMERICAL INTEGRATION FREE VIBRATION    |')
      fprintf('%s\n','|                   Central Difference Method    |')
      fprintf('%s\n','|                             K. D. Hjelmstad    |')
      fprintf('%s\n','*------------------------------------------------*')
      
%.... Input physical problem data
      stiff = 16.0;               % stiffness
      mass  = 1.0;                % mass 
      omega = sqrt(stiff/mass);   % natural frequency of SDOF system
      
%.... Initial position xo and velocity vo    
      xo = 3.0;
      vo = 10.0;
      
%.... Numerical analysis parameters
      dt = 1;                 % analysis time step
      to = 0.0;                   % initial time (usually zero)
      tf = 60;                  % final time (end of analysis)
      dtcrit = 2/omega;           % critical time step for stability
      
%.... Set up a history array to save results ever once in a while
%     This part is simply to avoid plotting every point that we compute
%     It does not take that many points to give a nice representative 
%     response curve. We need to compute more points to get accuracy in
%     the solution. 
      nitems  = 2;              % total number of items stored in history
      nreport = 500;            % number of steps to output
      nsteps  = ceil((tf-to)/dt) + 1; % total number of analysis time steps
      tf = nsteps*dt;
      nreport = min(nreport,nsteps);
      noutput = ceil(nsteps/nreport);
      history = zeros(nreport,nitems);
      
%.... Initialize position at n=0 'xold' and n=-1 'xoldold' 
      xold = xo;
      xoldold = (1 - omega^2*dt^2/2)*xo - dt*vo ;
      
%.... Echo input values to screen
      fprintf('\r\r%s\n',   '   PHYSICAL PROPERTIES           ')
      fprintf('%s%8.3f\n',  '   Stiffness                     ',stiff)
      fprintf('%s%8.3f\n',  '   Mass                          ',mass)
      fprintf('%s%8.3f\n',  '   Natural Frequency             ',omega)

      fprintf('\r\r%s\n',   '   INTEGRATION OF EQUATIONS      ')
      fprintf('%s%8.3f\n',  '   Initial time (to)             ',to)
      fprintf('%s%8.3f\n',  '   Final time (tf)               ',tf)
      fprintf('%s%8.3f\n',  '   Time increment (dt)           ',dt)
      fprintf('%s%8.3f\n',  '   Critical Time step            ',dtcrit)
      fprintf('%s%8i\n',    '   Number of time steps          ',nsteps)
      fprintf('%s%8i\n',    '   Number of steps to report     ',nreport)

      fprintf('\r\r%s\n',   '   INITIAL CONDITIONS            ')
      fprintf('%s%8.3f\n',  '   Initial position              ',xo)
      fprintf('%s%8.3f\n',  '   Initial velocity              ',vo)
      
%.... Set time and output counters
      t = to; out = 0; iout = 0;
      
%.... Compute motion by numerical integration--loop over time steps
      for i=1:nsteps
          
%....... Write the values out to history, if appropriate
%        This segment writes a row to 'history' every 'noutput' steps
         if (out==0)
           iout = iout + 1;
           history(iout,:)=[t,xold];
           out = noutput;
         end
         out = out - 1;

%....... Compute the values of the state for the next time step         
         t = t + dt;
         
%....... Compute new position from equation of motion combined with
%        the difference equation for acceleration
         xnew  = (2 - omega^2*dt^2)*xold - xoldold;
         
         
%....... Put current state (new) into old slot and old slot into the 
%        oldold slot to get ready for new step. 
         xoldold = xold;
         xold    = xnew;
         
      end % loop over time steps

%.... Put very last point into the history array                
      iout = iout + 1;
      history(iout,:)=[t,xold];      
      
%.... GRAPHICAL OUTPUT SECTION
%     In this section we create plots to display the results that were
%     stored in the array history. Each row of history contains the values
%     of the response at a given time. The value "iout" refers to the last
%     row number in history. Thus, history(1:iout,1) is time, etc.

%.... Compute comparison as exact classical solution
      t = 0; npts = 200;
      h = (tf - to)/(npts-1);
      b1 = 1 - (omega*dt)^2/2;
      b2 = omega*sqrt(1 - (omega*dt)^2/4);
      phi0 = atan(dt*b2/b1);
      phi1 = asin(dt*b2);
      phi2 = acos(b1);
      phi = max([phi0,phi2,phi2]);
      r = sqrt(b1^2 + (dt*b2)^2);      
      xcomp = zeros(3,npts);
      for i=1:npts
         nc = t/dt; amp = r^nc; angle = phi*nc;
         xcomp(1,i) = t;
         xcomp(2,i) = xo*cos(omega*t) + vo*sin(omega*t)/omega;
         xcomp(3,i) = amp*(xo*cos(angle) + vo*sin(angle)/b2);
         t = t + h;
      end
                
%.... Plot time history of response
      numerical = 1;      % =1 if you want the numerical soluton (black)
      classical = 0;      % =1 if you want the classical solution (red)
      numanaly  = 1;      % =1 if you wnat the analytical solution (blue)
      fig=figure(1); clf; grid on; axis square; hold on;
      xlabel('t'); ylabel('v'); title('Response vs. Time');
      if (classical==1)
         p = plot(xcomp(1,:),xcomp(2,:));
         set(p,'Color','red','LineWidth',2);
      end
      if (numanaly==1)
         p = plot(xcomp(1,:),xcomp(3,:));
         set(p,'Color','blue','LineWidth',1);
      end       
      if (numerical==1)
         p = plot(history(1:iout,1),history(1:iout,2),...
                     'o','MarkerFaceColor','k','Linewidth',1,...
                         'MarkerEdgeColor','k','MarkerSize',3);
      else
         p = plot(history(1:iout,1),history(1:iout,2));
         set(p,'Color','blue','LineWidth',1);
      end

      fprintf('\r%s\r',' Normal Termination of Program')
      
%.... End of program