package algorithms;

import java.util.Comparator;
import java.util.LinkedList;
import java.util.PriorityQueue;
import java.util.Scanner;

import graph.*;

public class PrimAlgorithm {

	public static <W extends Comparable<W>, D> Graph<W, D> primAlgorithm(Graph<W, D> in) {
		Graph<W, D> out = new Graph<>();

		PriorityQueue<Edge<W, D>> edges = new PriorityQueue<>(11, new Comparator<Edge<W, D>>() {
			public int compare(Edge<W, D> e1, Edge<W, D> e2) {
				return (e1.getWeight().compareTo(e2.getWeight()));
			}
		});

		LinkedList<Node<W, D>> nodes = new LinkedList<>(in.getNodes().values());

		while (out.getNumNodes() < in.getNumNodes()) {
			if(edges.isEmpty()) {
				Node<W, D> curr = nodes.remove((int) (Math.random() * (nodes.size() - 1)));
				out.addNode(curr.getId());
				edges.addAll(curr.getEdgeList());

			}
			if(edges.isEmpty()) {
				continue;
			}
			Edge<W, D> edge = edges.remove();

			if (out.getNodes().containsKey(edge.getStart().getId())
					&& out.getNodes().containsKey(edge.getEnd().getId())) {
				continue;
			} else if (out.getNodes().containsKey(edge.getStart().getId())) {
				out.addNode(edge.getEnd().getId());
				out.addEdge(new Edge<W, D>(out.getNodes().get(edge.getStart().getId()),
						out.getNodes().get(edge.getEnd().getId()), edge.getId(), edge.getWeight(), edge.isDirected()));
				nodes.remove(in.getNodes().get(edge.getEnd().getId()));
				edges.addAll(edge.getEnd().getEdgeList());
			} else {
				out.addNode(edge.getStart().getId());
				out.addEdge(new Edge<W, D>(out.getNodes().get(edge.getStart().getId()),
						out.getNodes().get(edge.getEnd().getId()), edge.getId(), edge.getWeight(), edge.isDirected()));
				nodes.remove(in.getNodes().get(edge.getStart().getId()));
				edges.addAll(edge.getStart().getEdgeList());
			}
		}
		return (out);
	}
	
	public static <W extends Comparable<W>, D> Graph<W, D> primAlgorithm(Graph<W, D> in, int start) {
		Graph<W, D> out = new Graph<>();

		PriorityQueue<Edge<W, D>> edges = new PriorityQueue<>(11, new Comparator<Edge<W, D>>() {
			public int compare(Edge<W, D> e1, Edge<W, D> e2) {
				return (e1.getWeight().compareTo(e2.getWeight()));
			}
		});

		LinkedList<Node<W, D>> nodes = new LinkedList<>();

		while (out.getNumNodes() < in.getNumNodes()) {
			if(edges.isEmpty()) {
				for(Node<W, D> n : in.getNodeList()) {
                    if(n.getId() == start) {
                        edges.addAll(n.getEdgeList());
                        out.addNode(n.getId());
                    } else {
                        nodes.add(n);
                    }
                }
			}
			if(edges.isEmpty()) {
				continue;
			}
			Edge<W, D> edge = edges.remove();

			if (out.getNodes().containsKey(edge.getStart().getId())
					&& out.getNodes().containsKey(edge.getEnd().getId())) {
				continue;
			} else if (out.getNodes().containsKey(edge.getStart().getId())) {
				out.addNode(edge.getEnd().getId());
				out.addEdge(new Edge<W, D>(out.getNodes().get(edge.getStart().getId()),
						out.getNodes().get(edge.getEnd().getId()), edge.getId(), edge.getWeight(), edge.isDirected()));
				nodes.remove(in.getNodes().get(edge.getEnd().getId()));
				edges.addAll(edge.getEnd().getEdgeList());
			} else {
				out.addNode(edge.getStart().getId());
				out.addEdge(new Edge<W, D>(out.getNodes().get(edge.getStart().getId()),
						out.getNodes().get(edge.getEnd().getId()), edge.getId(), edge.getWeight(), edge.isDirected()));
				nodes.remove(in.getNodes().get(edge.getStart().getId()));
				edges.addAll(edge.getStart().getEdgeList());
			}
		}
		return (out);
	}
    
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int n = in.nextInt();
        int m = in.nextInt();
        
        Graph<Integer, Integer> graph = new Graph<>(n);
        
        for(int i = 0; i < m;i++) {
            int a, b, c;
            a = in.nextInt();
            b = in.nextInt();
            c = in.nextInt();
            
            graph.addEdge(a - 1, b - 1, c);
        }
        Graph<Integer, Integer> out = primAlgorithm(graph, in.nextInt() - 1);
        
        int sum = 0;
        for(Edge<Integer, Integer> e : out.getEdgeList()) {
            sum += e.getWeight();
        }
        System.out.println(sum);
        in.close();
    }
}
