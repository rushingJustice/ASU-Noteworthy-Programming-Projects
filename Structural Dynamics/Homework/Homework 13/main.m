%   *------------------------------------------------------------*
%   |  Two or Three Dimensional Dynamic Non-Linear Truss Analysis|
%   |  Program. Includes prescribed forces and motions at DOF    |
%   |                                                            |                                                  
%   |  Author:  K. D. Hjelmstad                                  |
%   |  Date:    March 28, 2013                                   |
%   |                                                            |
%   |  Associated Files:                                         |
%   |     inputs.m   (data input)                                |
%   |     bound.m    (process boundary conditions)               |
%   |     loads.m    (build applied nodal load vector)           |
%   |     localize.m (compute element local quantities)          |
%   |     assemble.m (assemble stiffness matrix )                |
%   |     solve.m    (solve global equations)                    |
%   |     elem.m     (form element stiffness matrix)             |
%   |     stress.m   (compute element stresses)                  |
%   |     plotgeo.m  (plot problem geometry)                     |
%   |     plotdef.m  (plot deflected shape)                      |
%   |     outputs.m  (print results)                             |
%   |                                                            |
%   *------------------------------------------------------------*

      clear; clc;
      
%.... Print Banner
      fprintf('%s\n','*---------------------------------------*')
      fprintf('%s\n','|             TRUSS ANALYSIS PROGRAM    |')
      fprintf('%s\n','|                    K. D. Hjelmstad    |')
      fprintf('%s\n','|                                       |')
      fprintf('%s\n','|       Original Version: 03.28.2013    |')
      fprintf('%s\n','|          Latest Update: 03.28.2013    |')
      fprintf('%s\n','*---------------------------------------*')
      
%.... Numerical analysis parameters
      to     = 0;
      tf     = 50.0;               % Final time
      dt     = 0.01;              % analysis time step
      beta   = 0.25;              % Numark Method parameter
      eta    = dt^2*(1/2 - beta); % time integration parameter
      gamma  = 0.5;               % Numark Method parameter
      zeta   = dt*(1 - gamma);    % time integration parameter 
      nsteps = ceil((tf-to)/dt);  % number of time steps in analysis
      tf     = nsteps*dt;         % adjust final time to discrete times

      
%.... Input problem data (nodal coordinates, element connectivity, 
%     boundary cond, material sets, forces, etc.)
      [numdim,x,ix,id,d,f,numnod,numelm,nummat] = inputs3D;
      ndof = numnod*numdim;                  % Total number of DOF
      
%.... Change id array to be equations numbers for use in assembly routine
      [id,jpos] = bound(id,numdim,numnod);   % jpos is number of free DOF
      
%.... Echo input values to screen
      fprintf('\r\r%s\n', '   MODEL PROPERTIES              ')
      fprintf('%s%5i\n',  '   Dimension of space            ',numdim)
      fprintf('%s%5i\n',  '   Number of nodes               ',numnod)
      fprintf('%s%5i\n',  '   Number of members             ',numelm)
      fprintf('%s%5i\n',  '   Number of material sets       ',nummat)
      fprintf('%s%5i\n',  '   Number of degrees of freedom  ',ndof)
      fprintf('%s%5i\n',  '   Free DOF (jpos)               ',jpos)
%.... Mode toggle
      mode       = 0; % 1 <= mode <= jpos
      resonance  = 0; % =1 for resonance
      earthquake = 0; % =1 create earthquake force  
      
%.... Form applied load vector (and initialize forcing freq)
      [F] = loads(f,id,numnod,numdim,jpos);  % Retrieve loads F from  f
      Fo  = 400; % scale factor for loads
      Fwo = 0.5; % if no resonance
      
%.... Form mass matrix for dynamic analysis (nodal mass)
      M   = zeros(ndof,ndof);
      
      Mo  = 10; % Mass scaling value
      M   = Mo*eye(ndof);
      M   = M(1:jpos,1:jpos);

%.... Initialize position, velocity, and acceleration
      Xo    = 0.0;                % Scale for mode shape!
      Vo    = 0.00;                % Initial velocity scale
      Xi    = 0.0;                % Damping ratio scaling value
      
      damp  = Xi*ones(jpos,1); % modal damping
      xo    = Xo*ones(jpos,1); % initial displacement
      vo    = Vo*ones(jpos,1); % initial velocity
      aold  = zeros(jpos,1);
      
%.... Fake Earthquake motion       
      E00 = 0.1;                  % Damping factor for Eq acceleration
      E01 = 0.15;                 % Damping factor for Eq acceleration
      E11 = 0.05;                 % Amplitude for component 1
      E21 = 0.1;                  % Amplitude for component 2
      E31 = 0.2;                  % Amplitude for component 3
      E41 = 0.1;                  % Amplitude for component 4
      E12 = 1.0;                  % Frequency for component 1
      E22 = 3.0;                  % Frequency for component 2
      E32 = 7.0;                  % Frequency for component 3
      E42 = 15.0;                 % Frequency for component 4
      EVo = -0.00141;             % Initial ground velocity
      Eq  = 0;
      tD  = 50;                   % Duration of earthquake  
      Eqxold = 0;
      Eqvold = EVo;
      
% Set which figures to do (1=yes, 0=no)
      dofig5 = 1;                 % animation of motion
       sc    = 5;                     % Disp. magnification for plotting
       mtype = 4;                 % options for line type in plotgeo
       
%.... Set up a history array to save results ever once in a while
%     This part is simply to avoid plotting every point that we compute
%     It does not take that many points to give a nice representative 
%     response curve. We need to compute more points to get accuracy in
%     the solution. 
      hd1 = 1; hd2 = hd1+2;                    % space t,its,err
      hs1 = hd2+1; hs2 = hs1 + 3*jpos   - 1;   % space for x,v,a
      hp1 = hs2+1; hp2 = hp1 +   jpos   + 1;   % space for position, EQ
      hp3 = hp2+1; hp4 = hp3 + 4*jpos   - 1;   % space for di, Ni, up, yield
      hs5 = hp4+1; hs6 = hs5 + 1*numelm - 1;   % space for stresses
      nitems  = hs6;              % number of items stored in 'history'
      nreport = 1000;             % maximum number of steps reported
      nreport = min(nreport,nsteps);
      noutput = ceil(nsteps/nreport);
      history = zeros(nreport,nitems);        
      
%.... Initialize newton iteration parameters and initial parameters    
      t = to; its = 0; itmax = 10; tol = 10*10^-8; err = 1.0;
      out  = 0; iout  = 0;
      
%.... Earthquake acceleration
      if (earthquake==1)
          Eq = (E00*t*exp(-E01*t))*(E11*sin(E12*t)+ E21*sin(E22*t)...
                                   + E31*sin(E32*t)+ E41*sin(E42*t));
      else
          Eqxold = 0;
          Eqvold = 0;
          Eqaold = 0;
          Eqanew = Eqaold;
      end
     
      Eqaold = Eq;
      
%.... Ready the assemble subroutine for dynamic analysis      
      xold = zeros(jpos,1);
      up = F(jpos+1:ndof);
      u = [xold; up];
      
      [A,~,~] = assemble(x,ix,id,d,numdim,ndof,numelm,jpos,u); 
      
%.... Compute natrual frequencies and mode shapes
      freq = zeros(ndof,1);
      period = zeros(ndof,1);
      [V,D] = eig(A,M);

      Freq = sqrt(D)/(2*pi);
      freq = diag(Freq);
      period = freq.^(-1);  
      
%.... Compute the damping matrix from a modal definition of the damping
%.... matrix
      C = zeros(jpos,jpos);     
      CC = zeros(jpos,jpos);     
      for i = 1:jpos
          for j = 1:jpos
              if i == j
                  CC(i,j) = 2*Xi*sqrt(D(i,j));
              end % end if i == j
          end % end for j
      end % end for i  
      C = M*V*CC*V'*M;  
      
%.... Set initial displacement to Mode. Here you might want to provide the
%     ability to specify the initial displacement to be proportional to 
%     one of the mode shapes. The mode shapes are scaled by MATLAB to have
%     unit modal mass.
      if (mode < jpos+1) && (mode >= 1)
          xo = Xo*V(:,mode);
      end
      
%.... Allocate neccessary frequency for resonance based on mode shape
      if (resonance == 1) && (mode >=1) && (mode < jpos+1)
          Fwo = 2*pi*freq(mode);
          xo = zeros(jpos,1);
      end 
      
      F1   = Fo*sin(Fwo*t)*F(1:jpos); % sinusoidal forcing function
      
%.... Compute initial acceleration from the equation of motion  
      xold  = xo;
      vold  = vo;
      u = [xold; up];
      
      [A,g,r] = assemble(x,ix,id,d,numdim,ndof,numelm,jpos,u); 
      aold = M\( F1 - (g + C*vold));
      [bounds] = getplotbox(x,numdim);
     
%.... Create frame for animation if dofig option is on      
      if (dofig5==1)
        fig5=figure(5);                      % open figure
        numframes = 0;                       % initialize frame number
      end

%.... Compute motion by numerical integration
      for isteps=1:nsteps
          
%....... Write the values out to history, if appropriate
          if (out==0)
            [stresses] = stress(x,ix,id,d,u,numdim,numelm,jpos);
            iout = iout + 1;  
            history(iout,1:3)     = [t,its,err];
            history(iout,hs1:hs2) = [xold',vold',aold'];
            %history(iout, ADD u, Ne and others to history!
            history(iout,hs5:hs6) = stresses(:,2)';
  %....... Plot a frame of the animation, if option is on            
           if (dofig5==1)
             numframes = numframes + 1;
             rect = get(fig5,'Position'); rect(1:2) = [0 0];
             grid on; axis equal; axis (bounds); title('Motion'); hold on;
             plotdef(x,ix,id,u,stresses,d,numnod,numelm,numdim,...
                 jpos,sc,mtype);
             view(numdim);
             
             %.. Store frame for movie (set view point)  
             Mov(:,numframes) = getframe(fig5,rect); clf; 
          
           end % dofig5   
%........ Reset out to begin countdown until next output time            
            out = noutput;
            
         end % if out
         out = out - 1;   
            
%.... Compute the values for the next time step 
         t = t + dt;
         its = 0; err = 1.0;
         bn  = xold + dt*vold + beta*dt^2*aold;
         cn  = vold + dt*gamma*aold; 
         F1  = Fo*sin(Fwo*t)*F(1:jpos);
         anew = aold;
         
%.... Assemble stiffness matrix
      while (err > tol) && (its < itmax)
        its = its + 1;
        xnew  = bn + eta*anew;
        vnew  = cn +  zeta*anew;
        u = [xnew; up];    
        [A,g,r] = assemble(x,ix,id,d,numdim,ndof,numelm,jpos,u);
        g = M*anew + C*vnew + g - F1;
        A = M + C*zeta + A*eta;
        anew = anew - A\g;
        err = norm(g);
      end % while
      
%....... Compute velocity and displacement by numerical integration (Newm) 
      Eqvnew = Eqvold + dt*(gamma*Eqaold + (1 - gamma)*Eqanew);
      Eqxnew = Eqxold + dt*Eqvold + dt^2*(beta*Eqaold + (1/2 - beta)...
                                                                  *Eqanew);  
      vnew = vold + dt*(gamma*aold + (1 - gamma)*anew);
      xnew = xold + dt*vold + dt^2*(beta*aold + (1/2 - beta)*anew);
      
%....... Update state for next iteration   
      Eqaold = Eqanew;
      Eqvold = Eqvnew;
      Eqxold = Eqxnew;
      aold = anew;
      vold = vnew;
      xold = xnew;   
      
         
%.... Compute element forces from displacements      
      u = [xnew; up];   
      [stresses] = stress(x,ix,id,d,u,numdim,numelm,jpos);     

      end
      
%.... End of program Truss Analysis