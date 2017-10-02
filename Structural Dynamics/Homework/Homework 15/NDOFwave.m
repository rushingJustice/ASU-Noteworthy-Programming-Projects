%   *-------------------------------------------------------------------*
%   |  N DOF Axial Wave Equation                                        |  
%   |    This program computes the motion of the axial wave equation    |
%   |    with constant density and axial stiffness. The equations of    |
%   |    motion are integrated using Newmarks' method.                  |
%   |                                                                   |                                                  
%   |    The results are plotted as an animation in 2D.                 |                                                  
%   |                                                                   |                                                  
%   |  Author:  K. D. Hjelmstad                                         |
%   |  Date:    April 10, 2014vvv                                       |
%   |                                                                   |
%   |  Associated Files:                                                |
%   |     none                                                          |
%   *-------------------------------------------------------------------*

      clear; clc;
      
%.... Print Banner
      fprintf('%s\n','*------------------------------------------------*')
      fprintf('%s\n','|    Axial Wave Equation Finite Element Basis    |')
      fprintf('%s\n','|                             K. D. Hjelmstad    |')
      fprintf('%s\n','|                                                |')
      fprintf('%s\n','|               Original Version: 02.14.2012     |')
      fprintf('%s\n','|                  Latest Update: 02.16.2012     |')
      fprintf('%s\n','*------------------------------------------------*')
      
%---- INPUT PROBLEM DATA ------------------------------------------------*
      NDOF = 200;                 % number of degrees of freedom
%---- Start and end times for anaysis -----------------------------------*      
      to = 0.0;                   % Initial time (usu. zero)
      tf = 50.0;                  % Final time
      plotmax = 1.0;              % max and min y value for plot range
%---- Mass, stiffness, damping, length multipliers ----------------------*
      Mo = 1.0;                   % Mo = rho A Mass mass per unit length
      EA = 20.0;                  % EA stiffness scaling value
      Xi = 0.00;                  % Damping ratio scaling value
      Lo = 10.0;                  % Total length of bar
      fx_0 = 1;                   % =1 displacement equals zero, 0 free
      fx_L = 0;                   % =1 displacement equals zero, 0 free
      ne = fx_0 + fx_L;           % extra DOF due to fixed boundaries
      XNDOF = NDOF + ne;          % number of DOF for plotting
      dx = Lo/(XNDOF-1);          % element length
%---- Forcing function parameters ---------------------------------------*      
      Fo = 1.00;                  % Amplitude of forcing function
      Fwo = 2.0;                % Frequency of forcing function
      FDOF = NDOF;%ceil(NDOF/3);        % DOF to put forcing function
      FDOF = min(FDOF, NDOF);     % reset value if greater than nbodies
%---- Initial displacement and velocity multipliers ---------------------*
      Xo = 0.0;                   % Initial displacement scale
      Vo = 0.0;                   % Initial velocity scale
      Mode = 0;                   % if not 0 then initial displ is Mode
      resonance = 0;
%------------------------------------------------------------------------*

%.... Set which figures to do (1=yes, 0=no)      
      dofig1 = 1;                 % N displacements vs. time
      dofig2 = 0;                 % N velocities vs. time 
      dofig3 = 0;                 % unused
      dofig4 = 0;                 % unused
      dofig5 = 1;                 % animation of motion
        replay = 0;               % =1 query to replay movie
        makemovie = 0;            % write Npend.avi file for movie 
      
%.... Numerical analysis parameters
      dt     = 0.01;              % analysis time step
      beta   = 0.25;              % Newmark parameter
      gamma  = 0.5;               % Newmark parameter
      eta    = (1-gamma)*dt;      % time integration parameter
      zeta   = (0.5-beta)*dt^2;   % time integration parameter
      nsteps = ceil((tf-to)/dt);  % number of time steps in analysis
      tf     = nsteps*dt;         % adjust final time to discrete times
      tol    = 10e-8;             % Newton iteration tolerance
      itmax  = 10;                % maximum allowable Newton iterations      
      
%.... Initialize properties to constant arrays
      mass  = Mo*ones(NDOF,1)*dx/6;          
      stiff = EA*ones(NDOF,1)/dx;
      damp  = Xi*ones(NDOF,1);
      xo    = Xo*ones(NDOF,1);
      vo    = Vo*ones(NDOF,1);
                      
%.... Lay out the initial positions of each mass      
      xpo = Lo*ones(NDOF,1);
      for i=2:NDOF
        xpo(i) = xpo(i-1) + dx;   % initial positions
      end
      
%.... Compute stiffness matrix (linear finite element)  
      K = zeros(NDOF,NDOF);  
      K(1,1)       = stiff(1)*fx_0;
      K(NDOF,NDOF) = stiff(NDOF)*fx_L;
      for i=2:NDOF
          K(i-1,i-1) = K(i-1,i-1) + stiff(i);
          K(i-1,i)   = K(i-1,  i) - stiff(i);
          K(i,i-1)   = K(  i,i-1) - stiff(i);
          K(i,i)     = K(  i,  i) + stiff(i);
      end % for i
     
%.... Compute the mass matrix     
      M = zeros(NDOF,NDOF);     
      M(1,1)       = 2*mass(1)*fx_0;
      M(NDOF,NDOF) = 2*mass(NDOF)*fx_L;
     for i=2:NDOF
          M(i-1,i-1) = M(i-1,i-1) + 2*mass(i);
          M(i-1,i)   = M(i-1,  i) + mass(i);
          M(i,i-1)   = M(  i,i-1) + mass(i);
          M(i,i)     = M(  i,  i) + 2*mass(i);
      end % for i   
      
%.... Establish the shape of the applied force vector      
      force  = zeros(NDOF,1);
      force(FDOF) = 1.;            % Put force at degree of freedom FDOF
    
%.... Compute natrual frequencies and mode shapes    
      [V,D] = eig(K,M);
      freq = sqrt(diag(D));
      period = 2*pi./freq;

 %.... Compute the damping matrix     
      CC = zeros(NDOF,NDOF);     
      for i=1:NDOF
         CC(i,i) = 2*damp(i)*freq(i);
      end % for i        
      C = M*V*CC*V'*M;
      
%.... Set initial displacement to Mode or initial wave 
      if (Mode > 0) && (Mode <= NDOF)
         xo = Xo*V(:,Mode);
      else
         z0 = Lo/2 - Lo/10;
         z1 = Lo/2 + Lo/10;
         xo = zeros(NDOF,1);
         for i=1:NDOF
             z = Lo*(i/NDOF);
             if (z>z0) && (z<z1)
                 zi = (z-z0)/(z1-z0);
                 xo(i) = Xo*(1 - cos(2*pi*zi));
             end
         end
      end
%.... For resonance
      if resonance == 1;
      Fwo = freq(Mode);
      xo  = zeros(NDOF,1);
      end
      
%.... Initialize position and velocity
      xold = xo;
      vold = vo;          

%.... Set up a history array to save results ever once in a while
%     This part is simply to avoid plotting every point that we compute
%     It does not take that many points to give a nice representative 
%     response curve. We need to compute more points to get accuracy in
%     the solution.
      hd1 = 1; hd2 = hd1+2;               % space in history for data
      hs1 = hd2+1; hs2 = hs1+3*NDOF-1;    % space in history for x,v,a
      hp1 = hs2+1; hp2 = hp1 + NDOF+ne-1;    % space in history for position
      nitems  = hp2;              % number of items stored in 'history'
      nreport = 1000;             % maximum number of steps reported
      nreport = min(nreport,nsteps);
      noutput = ceil(nsteps/nreport);
      history = zeros(nreport,nitems);        

%.... Echo input values to screen
      fprintf('\r\r%s\n',   '   INTEGRATION OF EQUATIONS      ')
      fprintf('%s%8.3f\n',  '   Initial time (to)             ',to)
      fprintf('%s%8.3f\n',  '   Final time (tf)               ',tf)
      fprintf('%s%8.3f\n',  '   Time increment (dt)           ',dt)
      fprintf('%s%8.3f\n',  '   Numerical integration (beta)  ',beta)
      fprintf('%s%8.3f\n',  '   Numerical integration (gamma) ',gamma)
      fprintf('%s%8i\n',    '   Number of time steps          ',nsteps)
      fprintf('%s%8i\n',    '   Number of steps to report     ',nreport)

      fprintf('\r\r%s\n',   '   PHYSICAL PROPERTIES/INITIAL COND.')
      fprintf('%s%8i\n',    '   Number of DOF   :     ',NDOF)
      fprintf('\r%s'     ,  '    DOF     Mass    Stiff    Force      x-o')
      fprintf('%s\n'     ,  '      v-o     Damp     Freq   Period')

      for i=1:NDOF
        fprintf('%7i%9.3f%9.3f%9.3f',i, mass(i), stiff(i),force(i))
        fprintf('%9.3f%9.3f%9.3f',   xo(i), vo(i), damp(i))
        fprintf('%9.3f%9.3f\n',      freq(i), period(i))
      end    
      
%.... Set up initial parameters for first time step      
      t = to; its = 0; err = 1.0;
      out  = 0; iout  = 0; n = 0;

%.... Compute initial acceleration      
      Ft = Fo*sin(Fwo*t);
      aold = M\(Ft*force - K*xo - C*vo); 

%.... Compute motion by numerical integration
      for isteps=1:nsteps
          
%....... Write the values out to history, if appropriate
         if (out==0)
            iout = iout + 1;  
            history(iout,1:3) = [t,its,err];
            history(iout,hs1:hs2) = [xold',vold',aold'];
            x = xold;
            if (fx_0==1)
                x = [0; x];
            end
            if (fx_L==1)
                x = [x;0];
            end
            history(iout,hp1:hp2) = x';
            out = noutput;
         end
         out = out - 1;

%....... Compute the values for the next time step         
         t = t + dt;
         bn  = xold + dt*vold + beta*dt^2*aold;
         cn  = vold + gamma*dt*aold;
         
%....... Newton iteration to determine y (the acceleration at n+1)
         Ft = Fo*sin(Fwo*t);
         anew  = (M + eta*C + zeta*K)\(Ft*force - K*bn - C*cn);
         
%....... Compute velocity and displacement by numerical integration 
         vnew = vold + dt*(gamma*aold + (1-gamma)*anew);
         xnew = xold + dt*vold + dt^2*(beta*aold + (0.5-beta)*anew); 
         
%....... Update state for next iteration   
         aold = anew;
         vold = vnew;
         xold = xnew;           
         
      end

%.... GRAPHICAL OUTPUT OF RESULTS      
      zbeg = 4; zend = zbeg + NDOF -1;
      xminv = min(history(:,zbeg:zend)); xmaxv = max(history(:,zbeg:zend));
      xmin = min(xminv); xmax = max(xmaxv);
      ymin = -plotmax; ymax = plotmax;
      xmin = 0; xmax = Lo;
      pthick = ones(NDOF,1);            % thickness for trail
      for i=1:NDOF
          pcolor(i) = 'r';              % color for trail
      end
      pcolor(FDOF) = 'b';               % color for trail
      pthick(FDOF)  = 2;                % thickness for trail
      marker = floor(3 + 30/NDOF);      % marker size for hinges
   
%.... Diaplacements vs. time
      if (dofig1==1) 
        fig1 = figure(1); clf; grid on; axis square; hold on;
        xlabel('t'); ylabel('u');title('Displacements vs Time');
        for i=1:NDOF
          ix = 3+i;
          p = plot(history(1:iout,1),history(1:iout,ix));
          set(p,'Color',pcolor(i),'LineWidth',pthick(i));
        end
      end
      
%.... Velocities vs. time
      if (dofig2==1) 
        fig2 = figure(2); clf; grid on; axis square; hold on;
        xlabel('t'); ylabel('v');title('Velocity vs Time');
        for i=1:NDOF
          ix = 3+NDOF+i;
          p = plot(history(1:iout,1),history(1:iout,ix));
          set(p,'Color',pcolor(i),'LineWidth',pthick(i));
        end
      end 
      
%.... Plot trace of masses
      if (dofig4==1)
        fig4=figure(4); clf; 
        rect = get(fig4,'Position'); rect(1:2) = [0 0];
        grid on; axis square;
        xlabel('x'); ylabel('t'); title('Trace of masses');
        hold on;
        for i=1:NDOF
          p = plot(history(1:iout,hp1+i-1),history(1:iout,1));
          set(p,'Color',pcolor(i),'LineWidth',pthick(i));
        end
      end
 
      
%.... Plot 2D motion in space as a movie
      if (dofig5==1)
        fig5=figure(5);
        kframes = 3; kforget = 1; 
        numframes = floor(nreport/kframes);
        
        for k=1:numframes
          rect = get(fig5,'Position'); rect(1:2) = [0 0];
          grid on; axis ([xmin xmax ymin ymax]); axis square;
          xlabel('x'); ylabel('u'); title('Axial Displacement');
          hold on;
          
          kk = k - kforget; kk = (kk + abs(kk))/2;
          istart = 1 + kframes*kk; iend = min(kframes*k,iout);
          ix = hp1-1;
          xao = history(iend,hp1); yao = 0; ya = 0;
          for i=1:XNDOF

              ix = ix + 1;          
%.......... Plot the masses at the end of the trace period
            xa = history(iend,ix);  
            lny = [xao,xa]; lnx = [yao,ya];
            p = plot(lnx,lny,'-','Color','b','Linewidth',2);
            xao = xa; yao = ya;  ya = ya + dx;   
          end %  for i
          
%........ Put timer in upper left corner              
          time = num2str(history(iend,1),'%4.1f');             
          xplace = 0.8;
          yplace =  ymax*0.85;
          text(xplace,yplace,time,...
                      'FontSize',16,'HorizontalAlignment','left')          
                     
%........ Store frame for movie (set view point)        
          Mov(:,k) = getframe(fig5,rect);
          clf;
        end % for k

%...... Play (and record) movie with play speed factor faster that actual 
        
        clf; N=1; PlaySpeed = 1;
        FPS = ceil(PlaySpeed*numframes/tf);
        while (N~=0)
          movie(fig5,Mov, N,FPS,rect) 
           N = 0;
           if (replay==1)
             N = input('Replay times: ');
           end
        end
        if (makemovie==1)
          movie2avi(Mov,'NPend.avi','compression','None',...
                      'quality',100,'fps',FPS);   
        end
        
      end
  
      fprintf('\r%s\r',' Normal Termination of Program')
      
%.... End of program NBody