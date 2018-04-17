package algorithms;

import java.util.*;

import graph.*;

public class DijkstraAlgorithm {

	public static Graph<Double, Double> dijkstraAlgorithm(Graph<Double, Double> in, int start) {
		Graph<Double, Double> out = new Graph<>();

//		PriorityQueue<Node<Double, Double>> vset = new PriorityQueue<Node<Double, Double>>(11,
//				new Comparator<Node<Double, Double>>() {
//					public int compare(Node<Double, Double> a, Node<Double, Double> b) {
//						if (a.getData() > b.getData()) {
//							return (1);
//						} else {
//							return (-1);
//						}
//					}
//				});
		LinkedList<Node<Double, Double>> vset = new LinkedList<>();

		for (Node<Double, Double> n : in.getNodeList()) {
			Node<Double, Double> node;
			if (n.getId() != start) {
				node = new Node<Double, Double>(n.getId(), Double.POSITIVE_INFINITY);
			} else {
				node = new Node<Double, Double>(n.getId(), 0.0);
			}

			out.addNode(node);
			vset.add(node);
		}

		while (!vset.isEmpty()) {
			vset.sort(new Comparator<Node<Double, Double>>() {
					public int compare(Node<Double, Double> a, Node<Double, Double> b) {
						if (a.getData() > b.getData()) {
							return (1);
						} else {
							return (-1);
						}
					}
				});
			Node<Double, Double> u = vset.remove(), n = in.getNodes().get(u.getId());

			for (Edge<Double, Double> e : n.getEdgeList()) {
				if (e.traverse(u.getId()) != null) {
					Node<Double, Double> node = out.getNodes().get(e.traverse(u.getId()).getId());
					double alt = u.getData() + e.getWeight();
					if (alt < node.getData()) {
						node.setData(alt);
						if (node.getEdges().size() == 0) {
							out.addDirectedEdge(u.getId(), node.getId(), e.getWeight());
						} else {
							for (Edge<Double, Double> f : node.getEdgeList()) {
								if (f.traverse(node) == null) {
									out.deleteEdge(f);
									out.addDirectedEdge(u.getId(), node.getId(), f.getWeight());
						
								}
							}
						}
					}
				}
			}
		}

		return (out);
	}
	
	public static Graph<Double, Double> dijkstraPathAlgorithm(Graph<Double, Double> in, int start, int end) {
		Graph<Double, Double> out = new Graph<>();

		LinkedList<Node<Double, Double>> vset = new LinkedList<>();

		for (Node<Double, Double> n : in.getNodeList()) {
			Node<Double, Double> node;
			if (n.getId() != start) {
				node = new Node<Double, Double>(n.getId(), Double.POSITIVE_INFINITY);
			} else {
				node = new Node<Double, Double>(n.getId(), 0.0);
			}

			out.addNode(node);
			vset.add(node);
		}

		while (!vset.isEmpty()) {
			vset.sort(new Comparator<Node<Double, Double>>() {
					public int compare(Node<Double, Double> a, Node<Double, Double> b) {
						if (a.getData() > b.getData()) {
							return (1);
						} else {
							return (-1);
						}
					}
				});
			Node<Double, Double> u = vset.remove(), n = in.getNodes().get(u.getId());
			if(u.getId() == end) {
				break;
			}

			for (Edge<Double, Double> e : n.getEdgeList()) {
				if (e.traverse(u.getId()) != null) {
					Node<Double, Double> node = out.getNodes().get(e.traverse(u.getId()).getId());
					double alt = u.getData() + e.getWeight();
					if (alt < node.getData()) {
						node.setData(alt);
						if (node.getEdges().size() == 0) {
							out.addDirectedEdge(u.getId(), node.getId(), e.getWeight());
						} else {
							for (Edge<Double, Double> f : node.getEdgeList()) {
								if (f.traverse(node) == null) {
									out.deleteEdge(f);
									out.addDirectedEdge(u.getId(), node.getId(), f.getWeight());
						
								}
							}
						}
					}
				}
			}
		}
		
		Graph<Double, Double> output = new Graph<>();
		
		for(Node<Double, Double> n : out.getNodeList()) {
			output.addNode(n.getId());
		}
		
		Node<Double, Double> currn = out.getNodes().get(end);
		if(Double.isInfinite(currn.getData())) {
			return (output);
		}
		Edge<Double, Double> curre = null;
		
		for(Edge<Double, Double> e : currn.getEdgeList()) {
			if(e.getEnd() == currn) {
				curre = e;
				break;
			}
		}
		
				
		while(currn.getId() != start) {
			output.addDirectedEdge(curre.getStart().getId(), curre.getEnd().getId(), curre.getWeight());
			currn = curre.traverseUndirected(currn);
			for(Edge<Double, Double> e : currn.getEdgeList()) {
				if(e.traverse(currn) == null) {
					curre = e;
					break;
				}
			}
		}
		return (output);
	}

	public static void main(String[] args) {
		Scanner sc = new Scanner(System.in);

		String str = sc.nextLine();
		Graph<Double, Double> g = Graph.readString(str);

		int start = sc.nextInt() - 1;
		int end = sc.nextInt() - 1;
		
		System.out.println(dijkstraAlgorithm(g, start));
		System.out.print(dijkstraPathAlgorithm(g, start, end));

		sc.close();
	}
}
