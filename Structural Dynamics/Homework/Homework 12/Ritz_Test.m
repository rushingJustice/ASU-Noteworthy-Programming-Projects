clear all; clc; clf;
% Incoming Values %
M = [ 1 0 0 ; 0 2 0 ; 0 0 3];
K = [ 1 -2 0 ; -2 3 -4 ; 0 -4 4];
nbodies = 3;
nritz      = 3; % Number of ritz vectos (<<nbodies)
nrefine    = 1; % Number of subspace iterations

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
    [d, ind] = sort(d);
    for i = 1:nritz
        Qnew(:,i) = Qnew(:,ind(i));
        Qnew(:,i) = Qnew(:,i)/sqrt(Qnew(:,i)'*M*Qnew(:,i));   
    end
    
    Qold = Qnew;
end

% check sign with actual eigenvalues
for i = i:nritz*nbodies
    if sign(V(i)) > sign(Qnew(i))
        Qnew(i) = -1*Qnew(i);
    elseif sign(V(i)) < sign(Qnew(i))
        Qnew(i) = -1*Qnew(i);
    end
end     
A = zeros(nritz*nbodies,3);
A(:,2) = 1:nritz*nbodies;
A(:,1) = Qnew(:);
A(:,3) = V(:);
plot(A(:,2),A(:,1),'o',...
                'MarkerEdgeColor','k',...
                'MarkerFaceColor','r',...
                'MarkerSize',10)
hold on;
plot(A(:,2),A(:,3),'--bs','LineWidth',2)

    
    