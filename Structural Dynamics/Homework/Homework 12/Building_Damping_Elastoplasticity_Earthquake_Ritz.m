%   *--------------------------------------------------------------*
%   |  N DOF Vibration Problem.                                    |  
%   |    This program computes the motion of an N-DOF spring       |
%   |    mass system. The goal of this code are as follows:        |
%   |      1. NDOF shear building geometry                         |
%   |      2. Elasto-plastic elements                              |                                                  
%   |      3. Modal linear damping model                           |                                                  
%   |      4. Free vibration in natural modes                      |                                                  
%   |      5. Forced sinusoidal excitation at any story level      |                                                  
%   |      6. (Fake) Earthquake ground motion excitation           |
%   |      7. Eigenvector approximation (Ritz Vectors)             |   
%   *--------------------------------------------------------------*

      clear; clc;
      
%.... Print Banner
      fprintf('%s\n','*----------------------------------------------*')
      fprintf('%s\n','|                 N-DOF Strutural Vibration    |')
      fprintf('%s\n','|                           Michael Justice    |')
      fprintf('%s\n','|                                (Building)    |')
      fprintf('%s\n','|                                              |')
      fprintf('%s\n','|              Original Version: 03.11.2013    |')
      fprintf('%s\n','|                 Latest Update: 02.25.2015    |')
      fprintf('%s\n','*----------------------------------------------*')
      
%---- INPUT PROBLEM DATA ------------------------------------------------*
      nbodies = 25;                % number of DOF in system
      nritz   = 25;                % number of ritz vectos (<=nbodies)
      nrefine = 10;                 % number of subspace iterations
      rv      = 20;                 % ritz and eigenvector to plot (<=nritz)
      
%---- Start and end times for anaysis -----------------------------------*      
      to = 0.0;                   % Initial time (usu. zero)
      tf = 100.0;                 % Final time
      t = to;                      
      plotmax = 30;             % max and min x value for plot range
%---- Mass, stiffness, damping, length multipliers ----------------------*
      Mo = 50.0;                  % Mass scaling value
      Ko = 45.0;                  % Stiffness scaling value
      Xi = 0.05;                  % Damping ratio scaling value
      So = 50;                  % Yield scaling value
      Lo = 1.0;                   % basic length of springs (usually 1)
      di = zeros(nbodies,1);      % Displacement vector
      Ni = zeros(nbodies,1);      % Force in member
      up = zeros(nbodies,1);      % Plastic strain in each element
      yield = zeros(nbodies,1);   % Yield vector (binary) for plotting
      R  = zeros(nbodies,1);
      dr = zeros(nbodies,1);      
      dR = zeros(nbodies,nbodies);
      dU = zeros(nbodies,nbodies);
      T  = zeros(nbodies,nbodies);% Tangent Stiffness matrix
%---- Forcing function parameters ---------------------------------------*      
      resonance = 0;              % =0 for no resonance
      Fo1 = 0.0;                  % Constant forcing function
      Fo2 = 0.0;                  % Amplitude of forcing function
      Fwo = 0.3;                 % Frequency of forcing function
      FDOF = 5;                   % DOF to put forcing function
      FDOF = min(FDOF, nbodies);  % reset value if greater than nbodies
%---- Initial displacement and velocity multipliers ---------------------*
      Xo = 0.0;                   % Initial displacement scale
      Vo = 0.0;                   % Initial velocity scale
      mode = 1;                   % 1 <= mode <= nbodies
%---- Fake Earthquake motion --------------------------------------------*
      
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
      earthquake = 0;             % =1 create earthquake force
%------------------------------------------------------------------------*

%.... Set which figures to do (1=yes, 0=no)      
      dofig1 = 0;                 % N displacements vs. time
      dofig2 = 0;                 % N velocities vs. time 
      dofig3 = 0;                 % N accelerations vs. time
      dofig4 = 0;                 % static animation in 3D (time is 3rd D)
      dofig5 = 0;                 % animation of motion
        replay =0;               % =1 query to replay movie
        makemovie = 0;            % write Npend.avi file for movie 
      dofig6 = 0;                 % plot ground acceleration  
      dofig7 = 0;                 % slectively plot mode shapes  
      dofig8 = 0;                 % plot ground displacement  
      dofig9 = 0;                 % plot force vs. displacement  
      dofig10 = 1;                % plot ritz vector vs eigenvector 
      
%.... Numerical analysis parameters
      dt     = 0.01;              % analysis time step
      beta   = 0.25;               % Numark Method parameter
      eta    = dt^2*(1/2 - beta); % time integration parameter
      gamma   = 0.5;             % Numark Method parameter
      zeta   = dt*(1 - gamma);    % time integration parameter        
      nsteps = ceil((tf-to)/dt);  % number of time steps in analysis
      tf     = nsteps*dt;         % adjust final time to discrete times
      tol    = 10e-8;             % Newton iteration tolerance
      itmax  = 10;                % maximum allowable Newton iterations      
      
%.... Initialize properties to constant arrays
      model = 1;                  % =1 For model 
      ampm  = 0.2;                  % amplifier for model 
      mass  = Mo*ones(nbodies,1); % story mass       
      stiff = Ko*ones(nbodies,1); % story (or spring) stiffness
      damp  = Xi*ones(nbodies,1); % modal damping
      xo    = Xo*ones(nbodies,1); % initial displacement
      vo    = Vo*ones(nbodies,1); % initial velocity
      Si    = So*ones(nbodies,1); % yield force
      Eqxold = 0;
      Eqvold = EVo;

%.... Establish the form of the applied force for the sinusoidal force at
%     a certain level of the structure.
      F1    = zeros(nbodies,1);
      F2    = zeros(nbodies,1);
      force = zeros(nbodies,1);
      
%.... Compute the distribution of properties according to model
%     You may want to provide a better description of the distribution
%     of the properties. Put your modeling code here.
      if model == 1;   
        for  i = 1:nbodies
             mass(i)  = Mo/(i);
             stiff(i) = Ko/(i);
             damp(i)  = Xi*((i)+(i-1)*ampm);
             xo(i)    = Xo*((i)+(i-1)*ampm);
             vo(i)    = Vo*((i)+(i-1)*ampm);
        end
      end
%.... Lay out the initial (vertical) positions of each mass for the 
%     purposes of plotting the structure.
      xpo = Lo*ones(nbodies,1);
      for i=2:nbodies
        xpo(i) = xpo(i-1) + Lo;   % initial positions
      end
      
%.... Compute elastic stiffness matrix. The elastic stiffness is needed
%     to initialize the properties for the computation of the initial 
%     accelerations and to solve the eigenvalue problem to get the mode
%     shapes and frequencies. These can be used to compute the damping
%     matrix. It is a good idea to initialize arrays with the "zeros" 
%     command so that MATLAB does not think you are changing the size.
      K = zeros(nbodies,nbodies); 
      
%.... For the diagonals...
      for i = 1:nbodies
         for j = 1:nbodies
            if i == j && i < nbodies && j < nbodies
               K(i,j) = stiff(i) + stiff(i+1);
            elseif i == nbodies && j == nbodies
               K(i,j) = stiff(i);
            end      
         end
      end
 
 %.... For the off diagonals...
      for i = 1:nbodies
         for j = 1:nbodies
            if i == (j - 1)
               K(i,j) = -stiff(j);
            elseif i == (j + 1)
               K(i,j) = -stiff(i);
            end
         end
      end
 
%.... Compute the mass matrix. Same as above for K, except for mass
      M = zeros(nbodies,nbodies);     
      M = diag(mass);
            
%.... Establish the shape of the earthquake equivalent force vector      
      forceq = zeros(nbodies,1);
      if (earthquake==1)
          forceq = ones(nbodies,1);
          forceq = -M*forceq; % "Column of Ones" * M
      end
    
%.... Compute natrual frequencies and mode shapes. Note that "freq" and 
%     "period" are output quantities when inputs are echoed.
      freq = zeros(nbodies,1);
      period = zeros(nbodies,1);
      [V,D] = eig(K,M);

      Freq = sqrt(D)/(2*pi);
      freq = diag(Freq);
      period = freq.^(-1);
      
%.... Ritz Vectors
      [Qnew] = Ritz(M,K,nbodies,nritz,nrefine,V,rv,dofig10);
      
%.... Compute the damping matrix from a modal definition of the damping
%     matrix (you could alternatively do Rayleigh damping here).
      C = zeros(nbodies,nbodies);     
      CC = zeros(nbodies,nbodies);
      
      for i = 1:nbodies
          for j = 1:nbodies
              if i == j
                  CC(i,j) = 2*damp(i)*sqrt(D(i,j));
              end % end if i == j
          end % end for j
      end % end for i
      
      C = M*V*CC*V'*M;
      
%.... Set initial displacement to Mode. Here you might want to provide the
%     ability to specify the initial displacement to be proportional to 
%     one of the mode shapes. The mode shapes are scaled by MATLAB to have
%     unit modal mass.
      if (mode < nbodies+1) && (mode >= 1)
          xo = Mo*V(:,mode);
      end

%.... Compute the resonance in the building. Set the eigenvalues of the
%     system equal to the frequency of the forcing function to obtain
%     resonance in the mode shapes.
      if (resonance == 1) && (mode >=1) && (mode < nbodies+1)
          Fwo = 2*pi*freq(mode);
          xo = zeros(nbodies,1);
      end 
      
      if model == 1          
        for i = 1:nbodies
           F1(i) = Fo1*(sqrt(i))*ampm;      
           F2(i) = Fo2*(sqrt(i))*ampm;
           force(i) = F1(i) + F2(i)*sin(Fwo*t);
        end
      else
          force = (Fo1 + Fo2*sin(Fwo*t))*ones(nbodies,1);
      end
      
%.... Initialize position, velocity, and acceleration
      xold = xo;
      vold = vo;      
      
%.... Set up a history array to save results ever once in a while
%     This part is simply to avoid plotting every point that we compute
%     It does not take that many points to give a nice representative 
%     response curve. We need to compute more points to get accuracy in
%     the solution. 
      hd1 = 1; hd2 = hd1+2;                    % space t,its,err
      hs1 = hd2+1; hs2 = hs1 + 3*nbodies - 1;  % space for x,v,a
      hp1 = hs2+1; hp2 = hp1 +   nbodies + 1;  % space for position, EQ
      hp3 = hp2+1; hp4 = hp3 + 4*nbodies - 1;  % space for di, Ni, up, yield
      nitems  = hp4;              % number of items stored in 'history'
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
      fprintf('%s%8i\n',    '   Number of time steps          ',nsteps)
      fprintf('%s%8i\n',    '   Number of steps to report     ',nreport)

      fprintf('\r\r%s\n',   '   PHYSICAL PROPERTIES/INITIAL COND.')
      fprintf('%s%8i\n',    '   Number of masses  :     ',nbodies)
      %fprintf('%s%8.3f\n',  '   Frequency ratio   :     ',fratio)
      fprintf('\r%s'     ,  '    DOF     Mass    Stiff    Force      x-o')
      fprintf('%s\n'     ,  '      v-o  |  Damp     Freq   Period')

      for i=1:nbodies
        fprintf('%7i%9.3f%9.3f%9.3f',i, mass(i), stiff(i),force(i))
        fprintf('%9.3f%9.3f%9.3f',   xo(i), vo(i), damp(i))
        fprintf('%9.3f%9.3f\n',      freq(i), period(i))
      end    
      
%.... Set up initial parameters for first time step      
      its = 0; err = 1.0;
      out  = 0; iout  = 0;
      
%.... Compute strain (delta) and force (r) due to initial conditions.     
      
      for i = 1:nbodies
        if i == 1
            di(i) = xo(i);
        else
            di(i) = xo(i) - xo(i-1);
        end
      end
      
      for i = 1:nbodies
        Ni(i) = stiff(i)*di(i);
      end
     
      kT = stiff;
      
      for i = 1:nbodies
         if abs(Ni(i)) > Si(i)
            Ni(i) = Si(i)*sign(Ni(i));
            up(i) = di(i)-Ni(i)/stiff(i);
         end          
      end
      
      for i = 1:nbodies
            if i == nbodies
                R(i) = Ni(i);
            else
                R(i) = Ni(i) - Ni(i+1);
            end
      end 
            
%.... Compute initial acceleration from the equation of motion. Also 
%     compute the initial acceleration for integrating the earthquake
%     ground motion
      aold = zeros(nbodies,1);
      
      if (earthquake==1)
          Eq = (E00*t*exp(-E01*t))*(E11*sin(E12*t)+ E21*sin(E22*t)...
                                   + E31*sin(E32*t)+ E41*sin(E42*t));
      else
          Eqxold = 0;
          Eqvold = 0;
          Eqaold = 0;
          Eqanew = Eqaold;
      end
      
      aold = (M)\(force + forceq*Eq - (R + C*vo)); % Linear case
      Eqaold = Eq;
      
%.... Compute motion by numerical integration
      for isteps=1:nsteps
%....... Write the values out to history, if appropriate
         if (out==0)
            iout = iout + 1;  
            history(iout,1:3) = [t,its,err];
            history(iout,hs1:hs2) = [xold',vold',aold']; 
            Eqo = Eqxold;
            x = Eqo*ones(nbodies,1) + xold;
            history(iout,hp1:hp2) = [x',Eq,Eqo];
            history(iout,hp3:hp4) = [di',Ni',up',yield'];
            out = noutput;
         end
         out = out - 1;

%....... Compute the values for the next time step 
         t = t + dt; err = 1; its = 0;
         bn  = xold + dt*vold + beta*dt^2*aold;
         cn  = vold + dt*gamma*aold;
         
%....... Compute forcing function for the next time step      
         if model == 1;
            force = F1 + F2*sin(Fwo*t);
         else
            force  = Fo1*ones(nbodies,1) + Fo2*ones(nbodies,1)*sin(Fwo*t);
         end
         
         anew = aold;
         
%....... Newton iteration to determine y (the acceleration at n+1)
        if (earthquake==1)
            Eq = (E00*t*exp(-E01*t))*(E11*sin(E12*t)+ E21*sin(E22*t)...
                                   + E31*sin(E32*t)+ E41*sin(E42*t));
            Eqanew = Eq; 
            
            if (t>=tD)
                Eqvold = 0;
            end
            
        end

%....... Newton loop to find new accleleration from EOM         
         while (err > tol) && (its < itmax)
                its   = its + 1;
                xnew  = bn + eta*anew;
                vnew  = cn +  zeta*anew;
                
%......... Compute strain and force due to initial conditions      
           for i = 1:nbodies
               if i == 1
                di(i) = xnew(i);
               else
                di(i) = xnew(i) - xnew(i-1);
               end
           end
           
           for i = 1:nbodies
               Ni(i) = stiff(i)*(di(i)-up(i));
           end  
           kT = stiff;
%......... Compute the force and stiffness in each member 
           for i = 1:nbodies
            if abs(Ni(i)) > Si(i)
               Ni(i) = Si(i)*sign(Ni(i));
               kT(i) = 0;
            end          
           end
      
           for i = 1:nbodies
                 if i == nbodies
                    R(i) = Ni(i);
                 else
                    R(i) = Ni(i) - Ni(i+1);
                 end
           end  
            
%......... Compute the residual and tangent  


%......... Compute tangent stiffness matrix  
           for i = 1:nbodies
             for j = 1:nbodies
                if i == j
                   dR(i,j) = kT(i);
                   dU(i,j) = 1;
                end
                if i == j && i > 1
                   dR(i-1,j) = -kT(j);
                end
                if i == j && i < nbodies
                   dU(i+1,j) = -1;
                end
             end  
           end
           
           T = dR*dU;
           
%......... Compute new acceleration
           gp = M + C*zeta + T*eta;
           g  = M*anew + C*vnew + R - force + forceq*Eq;
           anew = anew - gp\g;
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

%....... If yielding occured (or continued) move the permanent set
         for i = 1:nbodies
             if (abs(Ni(i)) >= Si(i))
                 up(i) = di(i) - Ni(i)/stiff(i);
                 yield(i) = 1;
             end
         end
        end % loop over time steps
        
%.... GRAPHICAL OUTPUT OF RESULTS      
      zbeg = 4; zend = zbeg + nbodies -1;
      xminv = min(history(:,zbeg:zend)); xmaxv = max(history(:,zbeg:zend));
      xmin = min(xminv); xmax = max(xmaxv);
      xmin = -plotmax; xmax = plotmax;
      ymin = 0; ymax = nbodies*Lo;
      pthick = ones(nbodies,1);            % thickness for trail
      for i=1:nbodies-1
          pcolor(i) = 'r';                 % color for trail
      end
      pcolor(nbodies) = 'b';               % color for trail
      pthick(nbodies)  = 2;                % thickness for trail
      marker = floor(3 + 30/nbodies);      % marker size for hinges
   
%.... Diaplacements vs. time
      if (dofig1==1) 
        fig1 = figure(1); clf; grid on; axis square; hold on;
        xlabel('t'); ylabel('u');title('Displacements vs Time');
        for i=1:nbodies
          ix = 3+i;
          p = plot(history(1:iout,1),history(1:iout,ix));
          set(p,'Color',pcolor(i),'LineWidth',pthick(i));
        end
      end
      
%.... Velocities vs. time
      if (dofig2==1) 
        fig2 = figure(2); clf; grid on; axis square; hold on;
        xlabel('t'); ylabel('v');title('Velocity vs Time');
        for i=1:nbodies
          ix = 3+nbodies+i;
          p = plot(history(1:iout,1),history(1:iout,ix));
          set(p,'Color',pcolor(i),'LineWidth',pthick(i));
        end
      end 
      
%.... Accelerations vs. time
      if (dofig3==1) 
        fig3 = figure(3); clf; grid on; axis square; hold on;
        xlabel('t'); ylabel('a');title('Acceleration vs Time');
        for i=1:nbodies
          ix = 3+2*nbodies+i;
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
        for i=1:nbodies
          p = plot(history(1:iout,hp1+i-1),history(1:iout,1));
          set(p,'Color',pcolor(i),'LineWidth',pthick(i));
        end
      end
      
%.... Ground accelerations vs. time
      if (dofig6==1) 
        fig6 = figure(6); clf; grid on; axis square; hold on;
        xlabel('t'); ylabel('ag');title('Ground Acceleration vs Time');
        p = plot(history(1:iout,1),history(1:iout,hp2-1));
        set(p,'Color',pcolor(i),'LineWidth',pthick(i));
      end   
      
%.... Ground displacement vs. time
      if (dofig8==1) 
        fig8 = figure(8); clf; grid on; axis square; hold on;
        xlabel('t'); ylabel('ug');title('Ground Displacement vs Time');
        p = plot(history(1:iout,1),history(1:iout,hp2));
        set(p,'Color',pcolor(i),'LineWidth',pthick(i));
      end   
      
%.... Plot force-displacement response  
      if (dofig9==1)
        fig9=figure(9); clf; grid on; axis square; hold on;
        xlabel('Displ'); ylabel('F'); title('Force vs. displacement');
        for i=1:nbodies
          ix = hp3 + i - 1; iy = ix + nbodies;
          p = plot(history(1:iout,ix),history(1:iout,iy));
          set(p,'Color','blue','LineWidth',1); 
        end
      end % dofig9      
      
%.... Plot 2D motion in space as a movie
      if (dofig5==1)
        fig5=figure(5);
        kframes = 3; kforget = 1; 
        numframes = floor(nreport/kframes);
        
        for k=1:numframes
          rect = get(fig5,'Position'); rect(1:2) = [0 0];
          grid on; axis ([xmin xmax ymin ymax]); axis square;
          xlabel('x'); ylabel('y'); title('Trajectory');
          hold on;
          
          kk = k - kforget; kk = (kk + abs(kk))/2;
          istart = 1 + kframes*kk; iend = min(kframes*k,iout);
          ix = hp1;
          xao = history(iend,hp2); yao = 0;
          
%........ Plot the elements red=yielded, blue=elastic
          for i=1:nbodies
            ix =hp1 + i - 1; 
            ip = hp3 + 2*nbodies + i -1;
            upelem = history(iend,ip); lc = 'b';
            if (abs(upelem)>tol)
                lc = 'r';    
            end
            xa = history(iend:iend,ix); ya = i*Lo;
            lnx = [xao,xa]; lny = [yao,ya];
            p = plot(lnx,lny,'-','Color',lc,'Linewidth',2);
            xao = xa; yao = ya;
          end %  for i
          
%........ Plot the masses
          for i=1:nbodies
            ix =hp1 + i - 1; 
            xa = history(iend:iend,ix); ya = i*Lo;
            p = plot(xa,ya,'--rs','Color','b','Linewidth',2,...
                        'MarkerFaceColor','k',...
                        'MarkerEdgeColor','k',...
                        'MarkerSize',marker);
            xao = xa; yao = ya;
          end %  for i
                     
%........ Store frame for movie (set view point)        
          Mov(:,k) = getframe(fig5,rect);
          clf;
        end % for k

%...... Play (and record) movie with play speed factor faster that actual 
        
        clf; N=1; PlaySpeed = 2;
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