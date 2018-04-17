package graph;

import java.util.*;

public class Graph<W, D> {
	private HashMap<Integer, Edge<W, D>> edges;
	private HashMap<Integer, graph.Node<W, D>> nodes;
	private int nodeNum, edgeNum;
	private boolean connected, connectupdate;
	
	//General constructor.
	public Graph() {
		edges = new HashMap<>();
		nodes = new HashMap<>();
		nodeNum = 0;
		edgeNum = 0;
		connectupdate = true;
	}
	
	public Graph(int numNodes) {
		edges = new HashMap<>();
		nodes = new HashMap<>();
		nodeNum = 0;
		edgeNum = 0;
		
		for(int i = 0; i < numNodes; i++) {
			this.addNode();
		}
		connectupdate = true;
	}
	
	public Graph<W, D> clone() {
		Graph<W, D> out = new Graph<>();
		
		for(Node<W, D> n : this.nodes.values()) {
			out.addNode(new Node<W, D>(n.getId()));
		}
		
		for(Edge<W, D> e : this.edges.values()) {
			out.addEdge(new Edge<W, D>(out.nodes.get(e.getStart().getId()), out.nodes.get(e.getEnd().getId()), e.getId(), e.getWeight(), e.isDirected()));
		}
		out.connected = this.connected;
		out.connectupdate = this.connectupdate;
		
		return (out);
	}
	
//	public int addEdge(Edge<W> edge) {
//		edges.put(this.edgeNum++, edge);
//	}
	
	public int addEdge(int start, int end) {
		edges.put(this.edgeNum, new Edge<W, D>(nodes.get(start), nodes.get(end), this.edgeNum, false));
		this.edgeNum++;
		if(connectupdate == false && connected == false) {
			connectupdate = true;
		}
		return (this.edgeNum - 1);
	}
	
	public int addEdge(int start, int end, W weight) {
		edges.put(this.edgeNum, new Edge<W, D>(nodes.get(start), nodes.get(end), this.edgeNum, weight, false));
		this.edgeNum++;
		if(connectupdate == false && connected == false) {
			connectupdate = true;
		}
		return (this.edgeNum - 1);
	}
	
	public int addEdge(int start, int end, W weight, boolean directed) {
		edges.put(this.edgeNum, new Edge<W, D>(nodes.get(start), nodes.get(end), this.edgeNum, weight, directed));
		this.edgeNum++;
		if(connectupdate == false && connected == false) {
			connectupdate = true;
		}
		
		return (this.edgeNum - 1);
	}
	
	public int addDirectedEdge(int start, int end) {
		edges.put(this.edgeNum, new Edge<W, D>(nodes.get(start), nodes.get(end), this.edgeNum, true));
		this.edgeNum++;
		connectupdate = true;
		return (this.edgeNum - 1);
	}
	
	public int addDirectedEdge(int start, int end, W weight) {
		edges.put(this.edgeNum, new Edge<W, D>(nodes.get(start), nodes.get(end), this.edgeNum, weight, true));
		this.edgeNum++;
		if(connectupdate == false && connected == false) {
			connectupdate = true;
		}
		
		return (this.edgeNum - 1);
	}
	
	public int addNode() {
		nodes.put(this.nodeNum, new Node<W, D>(this.nodeNum));
		this.nodeNum++;
		connectupdate = false;
		connected = false;
		return(this.nodeNum - 1);
	}
	
	public int addNode(Node<W, D> node) {
		if(this.nodeNum < node.getId()) {
			this.nodeNum = node.getId() + 1;
		}
		nodes.put(node.getId(), node);
		connectupdate = false;
		connected = false;
		return (node.getId());
	}
	
	public HashMap<Integer, Edge<W, D>> getEdges() {
		return (this.edges);
	}
	
	public HashMap<Integer, Node<W, D>> getNodes() {
		return (this.nodes);
	}
	
	public Collection<Node<W, D>> getNodeList() {
		return (this.nodes.values());
	}
	
	public void deleteNode(int node) {
		Node<W, D> n = this.nodes.remove(node);
		
		for(Edge<W, D> e : n.getEdgeList()) {
			this.deleteEdge(e);
		}
		
		connectupdate = true;
		
	}
	
	public void deleteEdge(Edge<W, D> edge) {
		Edge<W, D> e = this.edges.remove(edge.getId());
		e.getStart().deleteEdge(e.getId());
		e.getEnd().deleteEdge(e.getId());
		connectupdate = true;
	}
	
	public void deleteEdge(int edge) {
		Edge<W, D> e = this.edges.remove(edge);
		e.getStart().deleteEdge(e.getId());
		e.getEnd().deleteEdge(e.getId());
		connectupdate = true;
	}
	
	public int getNumEdges() {
		return (this.edges.size());
	}
	
	public int getNumNodes() {
		return (this.nodes.size());
	}
	
	public int addEdge(Edge<W, D> e) {
		if(this.edgeNum < e.getId()) {
			this.edgeNum = e.getId() + 1;
		}
		this.edges.put(e.getId(), e);
		if(connectupdate == false && connected == false) {
			connectupdate = true;
		}
		return (e.getId());
	}
	
	public int addNode(int id) {
		this.nodes.put(id, new Node<W, D>(id));
		return (id);
	}
	
	/*
	 * [n e]{(s - e w), (s -> e w), ...}
	 */
	public static Graph<Double, Double> readString(String str) throws IllegalArgumentException {
		Graph<Double, Double> out = null;
		String[] strs = str.split("\\{|\\}|,");
		
		boolean start = false, end = false, weight = false, direction = false, dim = false;
		int startval = -1, endval = -1;
		double weightval = 0;
		boolean isDirected = false;
		
		for(String s : strs) {
			s = s.trim();
			switch(s.charAt(0)) {
			case '[':
				if(dim) {
					break;
				}
				String[] dims = s.split("\\[|\\]|[ \t\n]+");
				boolean node = false, edge = false;
				for(String t : dims) {
					if(t.isEmpty()) {
						continue;
					}
					if(Character.isDigit(t.charAt(0))) {
						if(!node) {
							node = true;
							out = new Graph<>(Integer.parseInt(t));
						} else if(!edge) {
							edge = true;
							Integer.parseInt(t);
							dim = true;
						}
					}
				}
				if(!dim) {
					throw(new IllegalArgumentException("Malformed input string. See documentation for proper graph input formatting."));
				}
				break;
			case '(':
				String[] vals = s.split("\\(|\\)|[ \t\n]|,");
				
				for(String t : vals) {
					if(!start) {
						try {
							startval = Integer.parseInt(t);
							start = true;
						} catch(NumberFormatException e) {
							continue;
						}
					} else if(!direction) {
						if(t.equals("-")) {
							direction = true;
							isDirected = false;
						} else if(t.equals("->")) {
							direction = true;
							isDirected = true;
						}
					} else if(!end) {
						try {
							endval = Integer.parseInt(t);
							end = true;
						} catch(NumberFormatException e) {
							continue;
						}
					} else if(!weight) {
						try {
							weightval = Double.parseDouble(t);
							weight = true;
						} catch(NumberFormatException e) {
							continue;
						}
					}
				}
				if(start && direction && end && weight) {
					out.addEdge(startval - 1, endval - 1, weightval, isDirected);
				}
				start = false;
				direction = false;
				end = false;
				weight = false;
			}
		}
		return (out);
	}
	
	@Deprecated
	public String toOutString() {
		return (this.toString());
	}

	/* (non-Javadoc)
	 * @see java.lang.Object#hashCode()
	 */
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + edgeNum;
		result = prime * result + ((edges == null) ? 0 : edges.hashCode());
		result = prime * result + nodeNum;
		result = prime * result + ((nodes == null) ? 0 : nodes.hashCode());
		return result;
	}

	/* (non-Javadoc)
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	@Override
	public boolean equals(Object obj) {
		if (this == obj) {
			return true;
		}
		if (obj == null) {
			return false;
		}
		if (!(obj instanceof Graph)) {
			return false;
		}
		
		@SuppressWarnings("unchecked")
		Graph<W, D> other = (Graph<W, D>) obj;
		if (edgeNum != other.edgeNum) {
			return false;
		}
		if (edges == null) {
			if (other.edges != null) {
				return false;
			}
		} else if (!edges.equals(other.edges)) {
			return false;
		}
		if (nodeNum != other.nodeNum) {
			return false;
		}
		if (nodes == null) {
			if (other.nodes != null) {
				return false;
			}
		} else if (!nodes.equals(other.nodes)) {
			return false;
		}
		return true;
	}
	
	@Override
	public String toString() {
		StringBuffer out = new StringBuffer();
		
		out.append("[");
		out.append(this.getNumNodes());
		out.append(' ');
		out.append(this.getNumEdges());
		out.append(']');
		out.append('{');
		
		for(Edge<W, D> e : this.getEdges().values()) {
			if(out.charAt(out.length() - 1) != '{') {
				out.append(", ");
			}
			out.append('(');
			out.append(e.getStart().getId());
			out.append(" -");
			if(e.isDirected()) {
				out.append('>');
			}
			out.append(' ');
			out.append(e.getEnd().getId());
			out.append(' ');
			out.append(e.getWeight());
			out.append(')');
		}
		out.append('}');
		return (out.toString());
	}
	
	@SuppressWarnings("unchecked")
	public boolean isConnected() {
		if(connectupdate == false) {
			return (connected);
		}
		LinkedList<Node<W, D>> open = new LinkedList<>(), closed = new LinkedList<>();
		open.add((Node<W, D>) this.getNodeList().toArray()[0]);
		
		while(!open.isEmpty()) {
			Node<W, D> curr = open.removeFirst();
			if(closed.contains(curr)) {
				continue;
			}
			
			LinkedList<Node<W, D>> neighbors = curr.getUndirectedNeighbors();
			
			for(Node<W, D> n : neighbors) {
				if(!closed.contains(n)) {
					open.add(n);
				}
			}
			closed.add(curr);
		}
		
		for(Node<W, D> n : this.getNodeList()) {
			if(!closed.contains(n)) {
				connectupdate = false;
				connected = false;
				return (false);
			}
		}
		connectupdate = false;
		connected = true;
		return (true);
	}
	
	public static Graph<Double, Double> randomGraph(int nodes, int edges) {
		Graph<Double, Double> out = new Graph<>(nodes);
		
		for(int i = 0; i < edges; i++) {
			int p1, p2;
			boolean direction;
			double weight;
			
			p1 = (int) (Math.random() * nodes);
			p2 = (int) (Math.random() * nodes);
			
			direction = ((Double.doubleToRawLongBits(Math.random()) & 1) == 0);
			
			weight = Math.random();
			
			out.addEdge(p1, p2, weight, direction);
		}
		return (out);
	}
	
	public static Graph<Double, Double> randomConnectedGraph(int nodes, int minEdges) {
		Graph<Double, Double> out = new Graph<>();
		
		out.addNode();
		
		while(out.getNumNodes() < nodes) {
			int start = (int) (Math.random() * out.getNumNodes());
			int end = out.addNode();
			double weight = Math.random();
			boolean direction = ((Double.doubleToRawLongBits(Math.random()) & 1) == 0);
			
			out.addEdge(start, end, weight, direction);
		}
		
		while(out.getNumEdges() < minEdges) {
			int p1, p2;
			boolean direction;
			double weight;
			
			p1 = (int) (Math.random() * nodes);
			p2 = (int) (Math.random() * nodes);
			
			direction = ((Double.doubleToRawLongBits(Math.random()) & 1) == 0);
			
			weight = Math.random();
			
			out.addEdge(p1, p2, weight, direction);
			
		}
	
		out.connected = true;
		out.connectupdate = true;	//Set to true, just in case I messed up.
		
		return (out);
	}
	
	public static Graph<Double, Double> randomUndirectedGraph(int nodes, int edges) {
		Graph<Double, Double> out = new Graph<>(nodes);
		
		for(int i = 0; i < edges; i++) {
			int p1, p2;
			double weight;
			
			p1 = (int) (Math.random() * nodes);
			p2 = (int) (Math.random() * nodes);
			
			weight = Math.random();
			
			out.addEdge(p1, p2, weight);
		}
		return (out);
	}
	
	public static Graph<Double, Double> randomConnectedUndirectedGraph(int nodes, int minEdges) {
		Graph<Double, Double> out = new Graph<>();
		
		out.addNode();
		
		while(out.getNumNodes() < nodes) {
			int start = (int) (Math.random() * out.getNumNodes());
			int end = out.addNode();
			double weight = Math.random();
			
			out.addEdge(start, end, weight);
		}
		
		while(out.getNumEdges() < minEdges) {
			int p1, p2;
			double weight;
			
			p1 = (int) (Math.random() * nodes);
			p2 = (int) (Math.random() * nodes);
			
			weight = Math.random();
			
			out.addEdge(p1, p2, weight);
			
		}
	
		out.connected = true;
		out.connectupdate = true;	//Set to true, just in case I messed up.
		
		return (out);
	}
	
	public Graph<W, D> randomWalk(int hops, int start) {
		if(this.getNode(start).getNumEdges() == 0) {
			return (null);
		}
		Graph<W, D> out = new Graph<>();
		
		for(Integer i : this.nodes.keySet()) {
			out.addNode(i);
		}
		
		Graph<W, D> temp = this.clone();
		
		out.addNode(start);
		Node<W, D> curr = temp.getNode(start);
		
l1:	for(int i = 0; i < hops; i++) {
			if(curr == null) {
				return (null);
			}
			LinkedList<Edge<W, D>> edges = new LinkedList<>(curr.getEdgeList());
			
			Edge<W, D> edge;
			do {
				if(edges.isEmpty()) {
					temp.deleteNode(curr);
					i = 0;
					out = new Graph<>();
					out.addNode(start);
					curr = temp.getNode(start);
					continue l1;
				}
				edge = edges.get((int) (edges.size() * Math.random()));
				if(edge.traverse(curr) == null) {
					temp.deleteEdge(edge.getId());
					edges.remove(edge);
				}
			} while(edge.traverse(curr) == null);
			
			if(edge.getStart() == curr) {
				out.addDirectedEdge(edge);
			} else {
				out.addDirectedEdge(edge.getEnd(), edge.getStart(), edge.getWeight());
			}
			curr = edge.traverse(curr);
		}
		return (out);
	}
	
	public Graph<W, D> randomWalk(int hops) {
		Graph<W, D> temp = this.clone(), out = null;
		
		do {
			int node = (int) (Math.random() * temp.getNumNodes());
			out = temp.randomWalk(hops, node);
			
			if(out == null) {
				temp.deleteNode(node);
			} else {
				return (out);
			}
		} while(out == null && temp.getNumNodes() != 0 && temp.getNumEdges() != 0);
		return (null);
	}
	
	public Node<W, D> getNode(int id) {
		return (this.nodes.get(id));
	}
	
	public void deleteNode(Node<W, D> node) {
		this.deleteNode(node.getId());
	}
	
	public int addDirectedEdge(Edge<W, D> edge) {
		return (this.addEdge(edge.getStart().getId(), edge.getEnd().getId(), edge.getWeight(), true));
	}
	
	public int addDirectedEdge(Node<W, D> start, Node<W, D> end, W weight) {
		return (this.addDirectedEdge(start.getId(), end.getId(), weight));
	}
	
	public Collection<Edge<W, D>> getEdgeList() {
		return (this.edges.values());
	}
	
	public int[][] toAdjacency() {
		int[][] out = new int[this.getNumNodes()][this.getNumNodes()];
		
		LinkedList<Node<W, D>> nodes = new LinkedList<>(this.getNodeList());
		
		for(Edge<W, D> e : this.getEdgeList()) {
			out[nodes.indexOf(e.getStart())][nodes.indexOf(e.getEnd())] = 1;
			if(!e.isDirected()) {
				out[nodes.indexOf(e.getEnd())][nodes.indexOf(e.getStart())] = 1;
			}
		}
		return (out);
	}
}
