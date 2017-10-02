clear all; clc;

% Parameters for Analytical and Classical Solution- %
w = 1; 
u_0 = 5;
v_0 = 0;
beta = 0.03;
gamma = 0.5;
% End of Parameters---------------------------------%

% Time Parameters-----------------------------------%
dt = 0.1;
tf = 30;
t0 = 0;
% End of Parameters---------------------------------%

% Numerical Analysis--------------------------------%
      nitems  = 4;               % total number of items stored in history
      nreport = 500;             % number of steps to output
      nsteps  = ceil((tf-t0)/dt); % total number of analysis time steps
      tf = nsteps*dt;
      nreport = min(nreport,nsteps);
      noutput = ceil(nsteps/nreport);
      history = zeros(nreport + 1,nitems);
      npts = 400;
      h = (tf - t0)/(npts - 1);
      
      
% Constants for Analytical, Numerical and Classical Solution-- %
      D1 = (w^2*dt^2*(beta - 1/2) - 1); 
      a = (w^2*dt^2*(2*beta - gamma + 1/2) - 2)/(2*D1);
      dot = (3/4*gamma*w^2*dt^2 - 1/16*w^2*dt^2 - 1/4*gamma^2*w^2*dt^2 ...
          - beta*w^2*dt^2 + 1);
      b = w*dt*sqrt(dot)/(D1);
      r2 = a^2 + b^2;
      phi = acos(a/(sqrt(r2)));
      u1s  = (u_0 + dt*v_0 - w^2*dt^2*beta*u_0)/(1 + w^2*dt^2*(1/2 - beta));
% End of Constants-------------------------------------------- %

% Numerical Part for Newmark's Method
%.... Initialize position, velocity, and put in 'xold' and 'vold' arrays
      xold = u_0;
      vold = v_0;
      
%.... Set time and output counters
      t = t0; out = 0; iout = 0;
      
%.... Compute initial acceleration from the equation of motion
      aold  = -w^2*u_0;
      
%.... Compute motion by numerical integration--loop over time steps
      for i=1:nsteps
          
%....... Write the values out to history, if appropriate
%        This segment writes a row to 'history' every 'noutput' steps
         if (out==0)
           iout = iout + 1;
           history(iout,:)=[t,xold,vold,aold];
           out = noutput;
         end
         out = out - 1;

%....... Compute the values of the state for the next time step         
         t = t + dt;
         
%....... Compute new acceleration from equation of motion 
         bn    = -(xold + dt^2*beta*aold + dt*vold);
         anew  = -w^2*bn/D1;
         
%....... Compute new velocity and position by Newmark         
         vnew   = vold + dt*(gamma*aold + (1-gamma)*anew);
         xnew   = xold + dt*vold + dt^2*(beta*aold + (1/2-beta)*anew);
         
%....... Put current state (new) into old slot to get ready for new step
%        Note that this approach allows us to only store two states as
%        we carry out the calculation: 'old' and 'new'
         aold = anew;
         vold = vnew;
         xold = xnew;
         
      end % loop over time steps

%.... Put very last point into the history array                
      iout = iout + 1;
      history(iout,:)=[t,xold,vold,aold];
      
%.... Plot Classical, Numerical and Analytical
t = t0;
ucomp = zeros(3,npts);
for i=1:npts
ucomp(1,i) = t;
ucomp(2,i) = (r2)^(t/(2*dt))*(u_0*cos(phi*t/dt) + (u1s - u_0*a)/(b)*sin(phi*t/dt));
ucomp(3,i) = u_0*cos(w*t) + v_0*sin(w*t)/w;
t = t + h;
end

 numerical = 1;      % =1 if you want the numerical soluton (black)
 classical = 1;      % =1 if you want the classical solution (red)
 numanaly  = 1;      % =1 if you want the analytical solution (blue)
 
fig=figure(1); clf; grid on; axis square; hold on;
xlabel('t'); ylabel('Disp.'); title('Displacement vs. Time');


if (numanaly  == 1)
    p = plot(ucomp(1,:),ucomp(2,:));
    hold on; set(p,'Color','blue','LineWidth',2);
end
if (classical == 1)
    p1 = plot(ucomp(1,:),ucomp(3,:));
    hold on; set(p1,'Color','red','LineWidth',0.5);
end

if (numerical  == 1)
    p = plot(history(1:iout,1),history(1:iout,2),...
        'o','MarkerFaceColor','k','Linewidth',1,...
        'MarkerEdgeColor','k','MarkerSize',3);
end

legend('Analytical','Classical','Numerical')