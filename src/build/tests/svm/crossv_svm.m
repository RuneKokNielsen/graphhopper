function result = crossv_svm(graphs, labels, kernel, nodeLabels, gaps)

labels = transpose(labels);

wValues = [0.001, 0.01, 0.1, 1, 10, 100, 1000];

    function result = get_k(w)
        result = graphhopper(graphs, kernel, nodeLabels, w, gaps);
    end

multiK = arrayfun(@ (x) (feval(@get_k, x)), wValues, 'un',0);

result = runntimes_multi(multiK, labels, 10);

end