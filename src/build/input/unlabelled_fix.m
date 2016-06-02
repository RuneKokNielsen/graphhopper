function result = unlabelled_fix(graphs)

s = size(graphs,2);
al = extractfield(graphs, 'al');

for i = 1:s
    graphs(1,i).nl = zeros(size(al{1,i},1),1);  
end

result = graphs;