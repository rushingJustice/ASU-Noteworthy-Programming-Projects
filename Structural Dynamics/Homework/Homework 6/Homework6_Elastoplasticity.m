clear; clc;

u_0 = 0;
v_0 = 0;
m = 1;
k = 5;
b = 0.8;
omega = 2;

% Time Parameters-----------------------------------%
dt = 1.0;
tf = 3.5;
t0 = 0;
t = t0;
% End of Parameters---------------------------------%

F1 = 0;
F2 = 2;
sig_0 = 1;

beta = 0.25; 
gamma = 0.5;
eta = dt^2*(1/2 - beta);

% Numerical Analysis--------------------------------%
      nitems  = 5;               % total number of items stored in history
      nreport = 500;             % number of steps to output
      nsteps  = ceil((tf-t0)/dt); % total number of analysis time steps
      tf      = nsteps*dt;
      nreport = min(nreport,nsteps);
      noutput = ceil(nsteps/nreport);
      history = zeros(nreport + 1,nitems);

% Numerical Part for Newmark's Method
%.... Initialize position, velocity, and put in 'xold' and 'vold' arrays
      xold = u_0;
      vold = v_0;

%.... Set time and output counters
      out = 0; iout = 0;
      
%.... Compute initial acceleration from the equation of motion
      F = F1 + F2*sin(omega*t);
      rold = k*u_0;
      aold  = (F - rold)/m;
      
%.... Compute motion by numerical integration--loop over time steps
      for i=1:nsteps
          
%....... Write the values out to history, if appropriate
%        This segment writes a row to 'history' every 'noutput' steps
         if (out==0)
           iout = iout + 1;
           history(iout,:)=[t,xold,vold,aold,rold];
           out = noutput;
         end
         out = out - 1;

%....... Compute the values of the state for the next time step         
         t = t + dt;
%....... Compute new acceleration from equation of motion using Newton
         n = 0;
         itmax = 20; tol = 1.e-8; err = 1.0;
         bn = xold + dt*vold + dt^2*beta*aold;
         F = F1 + F2*sin(omega*t);
         anew = aold;
         while ((err > tol) && (n < itmax))
                    n = n + 1;
                    xnew = bn + eta*anew;
                    r = k*xnew;
                    gp = m + k*eta; 
                    g  = m*anew + r - F;
                    anew = anew - g/gp;
                    err = norm(g);
          end 
%          anew = aold;
%          aold  = (F - rold*xold)/(m);
         
%....... Compute new velocity and position by Newmark         
         vnew   = vold + dt*(gamma*aold + (1-gamma)*anew);
         xnew   = xold + dt*vold + dt^2*(beta*aold + (1/2-beta)*anew);
         rtrial = rold + k*(xnew - xold);
             
         if abs(rtrial) > sig_0
                    rnew = sig_0*sign(rtrial);
         else
                    rnew   = rtrial;
         end

         %....... Put current state (new) into old slot to get ready for new step
%        Note that this approach allows us to only store two states as
%        we carry out the calculation: 'old' and 'new'
         aold = anew;
         vold = vnew;
         xold = xnew;
         rold = rnew;
         
      end % loop over time steps

%.... Put very last point into the history array                
      iout = iout + 1;
      history(iout,:)=[t,xold,vold,aold,rold];
      
%.... Plot displacement vs. time 
      fig=figure(1); clf; grid on; axis square; hold on;
      xlabel('t'); ylabel('Disp.'); title({'Displacement vs. Time'});
      p = plot(history(1:iout,1),history(1:iout,2),'-r');

%.... Plot force vs. displacement    
      fig=figure(2); clf; grid on; axis square; hold on;
      xlabel('Disp.'); ylabel('Force'); title({'Force vs. Displacement'});
      p = plot(history(1:iout,2),history(1:iout,5),'-');