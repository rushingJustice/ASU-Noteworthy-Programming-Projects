%   *-------------------------------------------------------------------*
%   |  N DOF Beam Wave Equation                                         |  
%   |    This program computes the motion of the BE-beam wave equation  |
%   |    with constant density and bending stiffness. The equations of  |
%   |    motion are integrated using Newmarks' method. The mass         |
%   |    includes rotary inertia.                                       |                                                               |                                                  
%   |                                                                   |                                                  
%   |    The results are plotted as an animation in 2D.                 |                                                  
%   |                                                                   |                                                  
%   |  Author:  K. D. Hjelmstad                                         |
%   |  Date:    April 19, 2014                                          |
%   |                                                                   |
%   |  Associated Files:                                                |
%   |     none                                                          |
%   *-------------------------------------------------------------------*

      clear; clc;
      
%.... Print Banner
      fprintf('%s\n','*------------------------------------------------*')
      fprintf('%s\n','|     Beam Wave Equation Finite Element Basis    |')
      fprintf('%s\n','|                             K. D. Hjelmstad    |')
      fprintf('%s\n','|                                                |')
      fprintf('%s\n','|               Original Version: 04.14.2014     |')
      fprintf('%s\n','|                  Latest Update: 04.19.2014     |')
      fprintf('%s\n','*------------------------------------------------*')
      
%---- INPUT PROBLEM DATA ------------------------------------------------*
      nElem = 100;                  % number of elements
      fx_0  = 1;                  % =1 displacement equals zero, 0 free
      fx_L  = 0;                  % =1 displacement equals zero, 0 free
      ne    = fx_0 + fx_L;        % number of restrained nodes
      nNode = nElem + 1;          % number of nodes
      rNode = nNode - ne;         % number of free nodes
      nDOF  = nNode*2;            % number of degrees of freedom
      rDOF  = nDOF - ne*2;        % free DOF
      repNDOF = 5;                % number of frequencies to print
%---- Start and end times for anaysis -----------------------------------*      
      to = 0.0;                   % Initial time (usu. zero)
      tf = 150.0;                  % Final time
      plotmax = 0.2;              % max and min y value for plot range
%---- Mass, stiffness, damping, length multipliers ----------------------*
      Lo      = 20.0;             % Total length of bar
      density = 10.0;              % mass density per unit volume
      modulus = 100.0;             % Young's modulus
      area    = 3.0;              % Cross sectional area
      inertia = 2.0;              % Second moment of area about centroid
      Xi      = 0.00;             % Damping ratio 
%---- Derived quantities ------------------------------------------------*      
      dx = Lo/nElem;              % element length
      Ma = density*area*dx;       % Ma = rho A = Mass per unit length
      Mi = density*inertia/dx;    % Mi = rho I = Inertia per unit length
      EI = modulus*inertia/dx^3;  % EI bending stiffness 
%---- Forcing function parameters ---------------------------------------*      
      Fo   = 0.1;                  % Amplitude of forcing function
      Fwo  = 3.00;              % Frequency of forcing function
      FDOF = ceil(rDOF/2);        % DOF to put forcing function
      FDOF = min(FDOF, rDOF);     % reset value if greater than rDOF
%---- Initial displacement and velocity multipliers ---------------------*
      Xo = 0.0;                   % Initial displacement scale
      Vo = 0.0;                   % Initial velocity scale
      Mode = 4;                   % if not 0 then initial displ is Mode
      resonance = 1;              % resonance
      wavefrac  = 20;             % Lo/wavefrac is size of initial wave
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
      a = dx; b = dx^2;
      mass  = [ 156,    22*a,    54,   -13*a;...
                 22*a,   4*b,    13*a,  -3*b;...
                 54,    13*a,   156,   -22*a;...
                -13*a,  -3*b,   -22*a,   4*b]/420;
            
      geom  = [  36,     3*a,   -36,     3*a;...
                  3*a,   4*b,    -3*a,  -1*b;...
                -36,    -3*a,    36,    -3*a;...
                  3*a,  -1*b,    -3*a,   4*b]/30;
              
      stiff = [  12,     6*a,   -12,     6*a;...
                  6*a,   4*b,    -6*a,   2*b;...
                -12,    -6*a,    12,    -6*a;...
                  6*a,   2*b,   -6*a,    4*b];
              
      mass  = Ma*mass + Mi*geom;          
      stiff = EI*stiff;
      
%.... Compute stiffness and mass matrices (cubic hermitian finite element)  
      K = zeros(nDOF,nDOF);  
      M = zeros(nDOF,nDOF);  
      for i=1:nElem
          ii = 2*(i-1) + 1; jj = ii + 3;
          K(ii:jj,ii:jj) = K(ii:jj,ii:jj) + stiff;
          M(ii:jj,ii:jj) = M(ii:jj,ii:jj) + mass;
      end % for i
      if (fx_0==1)
          K = K(3:nDOF,3:nDOF);
          M = M(3:nDOF,3:nDOF);
          nDOF = nDOF - 2;
      end
      if (fx_L==1)
          K = K(1:nDOF-2,1:nDOF-2);
          M = M(1:nDOF-2,1:nDOF-2);
          nDOF = nDOF - 2;
      end
           
%.... Establish the shape of the applied force vector      
      force  = zeros(rDOF,1);
      force(FDOF) = 1.;            % Put force at degree of freedom FDOF
      
                      
%.... Lay out the initial positions of each mass      
      xpo = Lo*ones(nDOF,1);
      for i=2:nDOF
        xpo(i) = xpo(i-1) + dx;   % initial positions
      end      
    
%.... Compute natrual frequencies and mode shapes    
      [V,D] = eig(K,M);
      freq = sqrt(diag(D));
      period = 2*pi./freq;

 %.... Compute the damping matrix     
      CC = zeros(nDOF,nDOF);     
      for i=1:nDOF
         CC(i,i) = 2*Xi*freq(i);
      end % for i        
      C = M*V*CC*V'*M;
      
%.... Set initial displacement to Mode or initial wave 
      vo = zeros(rDOF,1);
      if (Mode > 0) && (Mode <= rDOF)
         xo = Xo*V(:,Mode);
      else
         z0 = Lo/2 - Lo/wavefrac;
         z1 = Lo/2 + Lo/wavefrac;
         xo = zeros(rDOF,1);
         for i=1:rNode
             ii = 2*(i-1) + 1;   % translational DOF
             jj = ii + 1;        % rotational DOF
             z = Lo*(i/rNode);
             if (z>z0) && (z<z1)
                 zi = (z-z0)/(z1-z0);
                 xo(ii) = Xo*(1 - cos(2*pi*zi));
                 xo(jj) = Xo*2*pi*sin(2*pi*zi);
             end
         end
      end
%.... Set resonance here if called upon
      if resonance == 1
          Fwo = freq(Mode);
          xo  = zeros(rDOF,1);
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
      hs1 = hd2+1; hs2 = hs1+3*nDOF-1;    % space in history for x,v,a
      hp1 = hs2+1; hp2 = hp1 + nElem;    % space in history for position
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
      fprintf('%s%8i\n',    '   Number of DOF                 ',nDOF)
      fprintf('%s%8.3f\n',  '   Density                       ',density)
      fprintf('%s%8.3f\n',  '   Modulus                       ',modulus)
      fprintf('%s%8.3f\n',  '   Area                          ',area)
      fprintf('%s%8.3f\n',  '   Inertia                       ',inertia)
      fprintf('%s%8.3f\n',  '   Damping ratio                 ',Xi)
      fprintf('%s%8.3f\n',  '   Force Amplitude               ',Fo)
      fprintf('%s%8.3f\n',  '   Force Frequency               ',Fwo)
      fprintf('%s%8.3f\n',  '   DOF of applied force          ',FDOF)
      fprintf('%s%8.3f\n',  '   Amplitue of initial disp.     ',Xo)
      fprintf('%s%8i\n',    '   Mode                          ',Mode) 

      fprintf('\r%s\n'     ,  '    Mode       Freq     Period')

      for i=1:repNDOF
        fprintf('%8i%11.5f%11.5f\n', i,freq(i), period(i))
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
            x = zeros(rNode,1);
            for ii=1:rNode
                i = 2*(ii-1) + 1;
                x(ii) = xold(i);
            end
            if (fx_0==1)
                x = [0; x];
            end
            if (fx_L==1)
                x = [x;0];
            end
            [XNDOF,~] = size(x);

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
      zbeg = 4; zend = zbeg + nDOF -1;
      xminv = min(history(:,zbeg:zend)); xmaxv = max(history(:,zbeg:zend));
      xmin = min(xminv); xmax = max(xmaxv);
      ymin = -plotmax; ymax = plotmax;
      xmin = 0; xmax = Lo;
      pthick = ones(nDOF,1);            % thickness for trail
      for i=1:nDOF
          pcolor(i) = 'r';              % color for trail
      end
      pcolor(FDOF) = 'b';               % color for trail
      pthick(FDOF)  = 2;                % thickness for trail
      marker = floor(3 + 30/nDOF);      % marker size for hinges
   
%.... Diaplacements vs. time
      if (dofig1==1) 
        fig1 = figure(1); clf; grid on; axis square; hold on;
        xlabel('t'); ylabel('w');title('Displacements vs Time');
        for i=1:nDOF
          ix = 3+i;
          p = plot(history(1:iout,1),history(1:iout,ix));
          set(p,'Color',pcolor(i),'LineWidth',pthick(i));
        end
      end
      
%.... Velocities vs. time
      if (dofig2==1) 
        fig2 = figure(2); clf; grid on; axis square; hold on;
        xlabel('t'); ylabel('v');title('Velocity vs Time');
        for i=1:nDOF
          ix = 3+nDOF+i;
          p = plot(history(1:iout,1),history(1:iout,ix));
          set(p,'Color',pcolor(i),'LineWidth',pthick(i));
        end
      end 
      
%.... Plot trace of masses
      if (dofig4==1)
        fig4=figure(4); clf; 
        rect = get(fig4,'Position'); rect(1:2) = [0 0];
        grid on; axis square;
        xlabel('w'); ylabel('t'); title('Trace of masses');
        hold on;
        for i=1:nDOF
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
          xlabel('x'); ylabel('w'); title('Transverse Displacement');
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
            p = plot(lnx,lny,'-','Color','b','Linewidth',3);
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
      
%.... End of program NDOFBeamWave