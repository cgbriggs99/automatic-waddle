package tests;

import algorithms.*;
import graph.*;

public class RandomGraphs {

	
	public static void main(String args[]) {
		Graph<Double, Double> g = Graph.randomConnectedGraph(20, 30);
		
		System.out.println(g);
		System.out.println(GraphAlgs.primAlgorithm(g));
		System.out.println(GraphAlgs.dijkstraAlgorithm(g, 0));
		System.out.println(g.randomWalk(10));
	}
}
