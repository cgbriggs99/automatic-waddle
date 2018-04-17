package algorithms;

import graph.*;

public class GraphAlgs {
	public static <W extends Comparable<W>, D> Graph<W, D> primAlgorithm(Graph<W, D> in) {
		return (PrimAlgorithm.primAlgorithm(in));
	}
	
	public static Graph<Double, Double> dijkstraAlgorithm(Graph<Double, Double> in, int start) {
		return (DijkstraAlgorithm.dijkstraAlgorithm(in, start));
	}
	
	public static Graph<Double, Double> dijkstraPathAlgorithm(Graph<Double, Double> in, int start, int end) {
		return(DijkstraAlgorithm.dijkstraPathAlgorithm(in, start, end));
	}
}
