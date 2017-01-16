function [U] = calculate(depl)
	load("content");

	%imposition des conditions en deplacement : traction simple
	BND = [left  right left(1)+Nnodes];
	VALS = [zeros(size(left)) depl*ones(size(right)) 0];
	%on bloque U sur left
	%on impose U à 1 sur right
	%on bloque V(left(1))

	%modification de la matrice
	b = zeros(2*Nnodes,1);
	K(BND,:) = 0;
	b(BND) = VALS;
	K(BND,BND) = speye(numel(BND));

	%solution
	U = K\b;
	U = reshape(U,Nnodes,2);

	%affichage
	clf();
	graphics_toolkit("gnuplot");
	f = figure('visible','off');
	axis([-10,60,-20,30], 'equal','off'); 
	f = patch('vertices',XY+reshape(U,Nnodes,2),'faces',T20,'cdata',U);
	saveas(f,"im.jpg");
endfunction
