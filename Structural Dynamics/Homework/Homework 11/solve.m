%------------------------------------------------------------------solve
      function [u,r] = solve(A,F,jpos,ndof)
%   *-----------------------------------------------------------------*
%   |                                                                 |
%   |   SOLVE: Compute the diplacement and reactions from global      |
%   |   equations.                                                    |
%   |                                                                 |
%   |   Incoming values:                                              |
%   |      A = global stiffness matrix                                |
%   |      F = global force vector                                    |
%   |      jpos = pointer dividing free DOF from constrained DOF      |
%   |      ndof = total number of DOF                                 |
%   |                                                                 |
%   |   Outgoing values:                                              |
%   |      u = global displacements                                   |
%   |      r = net nodal forces forces                                |
%   |                                                                 |
%   *-----------------------------------------------------------------*
      
%.... Partition force vector into applied forces and known motions
      fp  = F(1:jpos);           % Prescribed loads 
      up  = F(jpos+1:ndof);      % Prescribed displacements 
      
%.... Partition stiffness matrix A = [Aff, Afp; Apf, App];
      Aff = A(1:jpos,1:jpos);      Afp = A(1:jpos, jpos+1:ndof);
      Apf = A(jpos+1:ndof,1:jpos); App = A(jpos+1:ndof, jpos+1:ndof);
             
%.... Solve equations and update solution
      b = fp - Afp*up;          % Compute rhs
      uf = Aff\b;               % Compute displacement (free DOF only)
      
%.... Compute reaction forces      
      reac = Apf*uf + App*up;   % Compute reactions (net force at each node)      u = [uf; up];             % Concatenate free/fixed displacements

%.... Concatenate the displacement and net force vectors      
      u = [uf; up];             % Concatenate free/fixed displacements
      r = [ fp ; reac ];        

      return