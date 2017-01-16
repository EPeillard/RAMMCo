close all
clear all

%Geometrie et maillage : un rectangle, mais il faut juste lire le fichier
%maillage pour faire autre chose.
%longueurs
Lx = 45;
Ly = 10;
%discr�tisation
Nx = 21;
Ny=11;
dx = Lx/(Nx-1);
dy = Ly/(Ny-1);
h = min(dx,dy);
%generation des points
[XX,YY] = meshgrid(linspace(0,Lx,Nx),linspace(0,Ly,Ny));
%bruit sur les noeuds interieurs afin de "flipper" certains triangles
XX(2:end-1,2:end-1) = XX(2:end-1,2:end-1) + h*1e-3*randn(size(XX(2:end-1,2:end-1)));
YY(2:end-1,2:end-1) = YY(2:end-1,2:end-1) + h*1e-3*randn(size(YY(2:end-1,2:end-1)));

%generation du maillage
XY = [XX(:),YY(:)];
T20 = delaunay(XY);

%definitions + frontieres
Nnodes=size(XY,1);
Nelem = size(T20,1);

%noeuds des 4 bords
left = 1:Ny;
right = (Nnodes- Ny+1):Nnodes;
down = 1:Ny:(Nnodes-Ny+1);
up = Ny:(Ny):Nnodes;

%affichage du maillage
%figure;
%hold on
%patch('vertices',XY,'faces',T20,'facecolor','none','edgecolor','b');
%plot(XY(down,1),XY(down,2),'ro');
%plot(XY(up,1),XY(up,2),'ro');
%plot(XY(left,1),XY(left,2),'mx');
%plot(XY(right,1),XY(right,2),'mx');

%comportement 
E = 1;
nu=0.3;
A = E/(1-nu^2);
B = (E*nu)/(1-nu^2);
C = E/(2*(1+nu));

%Construction des matrices de differentiation et des poids
%Dx calcule sur chaque element la derivee en x d'un champ scalaire U: 
%du/dx = Dx*U
%idem pour Dy suivant y
Dx = sparse(Nelem,Nnodes);
Dy = sparse(Nelem,Nnodes);
%W donne la surface de chaque element
W = zeros(Nelem,1);
%gradients des fonctions de forme sur l'element parent
dphidxi = [-1 -1;1 0; 0 1];
for e=1:Nelem
    connec = T20(e,:);
    %noeuds de l'element
    XY_loc = XY(connec,:);
    %matrice Jacobienne
    J = XY_loc'*dphidxi;
    %poids d'integration
    if det(J)<0
        T20(e,:) = fliplr(connec);
    end
end
for e=1:Nelem
    %connectivite locale
    connec = T20(e,:);
    %noeuds de l'element
    XY_loc = XY(connec,:);
    %matrice Jacobienne
    J = XY_loc'*dphidxi;
    %poids d'integration
    W(e) = det(J)/2;
    %derivees des fcts de forme
    dphidX = dphidxi*inv(J); %inv c'est moche,...
    %assemblage de Dx et Dy
    Dx(e,connec) = dphidX(:,1)';
    Dy(e,connec) = dphidX(:,2)';
end
%On transforme W en matrice diagonale,...cf la suite
W = spdiags(W,0,Nelem,Nelem);

%Assemblage du probl�me d'�lasticit�
%idU: indices des valeurs nodales de deplacement suivant x
idU = 1:Nnodes;
%idV: indices des valeurs nodales de deplacement suivant y
idV = idU+Nnodes;
%NB: on numerote d'abord U puis V

%initialisation de la matrice de raideur
K = sparse(2*Nnodes,2*Nnodes);
%assemblage des differents termes
K(idU,idU) = A*Dx'*W*Dx + C*Dy'*W*Dy;
K(idV,idV) = A*Dy'*W*Dy + C*Dx'*W*Dx;
K(idU,idV) = B*Dx'*W*Dy + C*Dy'*W*Dx;
K(idV,idU) = B*Dy'*W*Dx + C*Dx'*W*Dy;

%imposition des conditions en deplacement : traction simple
BND = [left  right left(1)+Nnodes];
VALS = [zeros(size(left))  ones(size(right)) 0];
%on bloque U sur left
%on impose U � 1 sur right
%on bloque V(left(1))

%modification de la matrice
b = zeros(2*Nnodes,1);
K(BND,:) = 0;
b(BND) = VALS;
K(BND,BND) = speye(numel(BND));

%solution
U = K\b;
U = reshape(U,Nnodes,2);

eps_xx = 1/Lx;
eps_yy = U(up(1),2)/Ly;
%on verifie que eps_yy = -eps_xx*nu 
fprintf('traction simple:\n eps_yy=%g  et -eps_xx*nu=%g\n\n',eps_yy, -eps_xx*nu);


%affichage
figure;
hold on
patch('vertices',XY,'faces',T20,'edgecolor','b','facecolor','none');
patch('vertices',XY+reshape(U,Nnodes,2),'faces',T20,'edgecolor','r','facecolor','none');
axis equal
