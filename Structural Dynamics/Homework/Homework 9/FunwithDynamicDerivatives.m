k1 = sym('k1');k2 = sym('k2');k3 = sym('k3');
dR = [k1 -k2 0 ; 0 k2 -k3 ; 0 0 k3]
dDelta = [ 1 0 0 ; -1 1 0 ; 0 -1 1]
R = dR*dDelta
