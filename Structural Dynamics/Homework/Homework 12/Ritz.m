function [Qnew] = Ritz(M,K,nbodies,nritz,nrefine,V,rv,dofig10)
% Incoming Values %

% End Incoming Values %

one   = ones(nbodies,1);
phi   = zeros(nbodies,nritz);
phi_t = zeros(nbodies,nritz);
sum_c = zeros(nbodies,nritz-1);
Qold  = zeros(nbodies,nritz);

% prep initial ritz vectors
phi(:,1) = K\M*one;
phi_t(:,1) = phi(:,1);

% generalize GS algorithm (Make sure to sum the GS coefficients....)
for i = 1:nritz
    for j = 1:i-1
        phi(:,i) = K\M*phi_t(:,i-1);
        sum_c(:,j) = phi_t(:,j)'*M*phi(:,i)/...
            (phi_t(:,j)'*M*phi(:,j))*phi_t(:,j);
    
    end
        phi_t(:,i) = phi(:,i) - sum(sum_c,2);
end

% normalize orthogonalized Ritz vectors and flip sign
for i = 1:nritz
    phi_t(:,i) = phi_t(:,i)/sqrt(phi_t(:,i)'*M*phi_t(:,i));   
end

% subspace iteration
Qold = phi_t;
for j = 1:nrefine
    Q = K\M*Qold;
    M_h = Q'*M*Q;
    K_h = Q'*K*Q;
    [VV,DD] = eig(K_h,M_h);
    phi_c = VV;
    Qnew = Q*phi_c;
   
    d = diag(DD);
    [~, ind] = sort(d);
    
    for i = 1:nritz
        Qnew(:,i) = Qnew(:,ind(i));
        Qnew(:,i) = Qnew(:,i)/sqrt(Qnew(:,i)'*M*Qnew(:,i));   
    end
    
    Qold = Qnew;
end

Qnew = Qold; % If zero subspace iterations, otherwise Qnew is Qold 

% check sign with actual eigenvalues
for i = 1:nritz*nbodies
    if sign(V(i)) > sign(Qnew(i))
        Qnew(i) = -1*Qnew(i);
    elseif sign(V(i)) < sign(Qnew(i))
        Qnew(i) = -1*Qnew(i);
    end
end    



% Assemble dof vector and check rv
dof = zeros(nbodies,1);

for i = 1:nbodies
dof(i) = i;
end

% Plot
if (dofig10 == 1)
    
if rv > 0
clf; fig10 = figure(10);
plot(dof(:),V(:,rv),'--',...
                'MarkerEdgeColor','k',...
                'MarkerFaceColor','r',...
                'MarkerSize',5)
            
hold on;
plot(dof(:),Qnew(:,rv),'o',...
                'MarkerEdgeColor','r',...
                'MarkerFaceColor','r',...
                'MarkerSize',5)
if nrefine > 0
title(sprintf('Subspace Iterations = %f', nrefine))   
end
xlabel('DOF') 
ylabel('Vector Space')
if nrefine > 0
legend('Eigenvector','Refined Ritz Vector','Location','southwest')
else
legend('Eigenvector','Ritz Vector','Location','southwest')
end
end
end

    
    