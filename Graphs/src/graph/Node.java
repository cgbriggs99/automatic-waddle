package graph;

import java.util.*;

public class Node<W, D> {
	private HashMap<Integer, Edge<W, D>> edges;
	private int id;
	private D data;
	
	public Node(int id) {
		this.id = id;
		this.edges = new HashMap<>();
		this.data = null;
	}
	
	public Node(int id, D data) {
		this.id = id;
		this.edges = new HashMap<>();
		this.data = data;
	}
	
	public HashMap<Integer, Edge<W, D>> getEdges() {
		return (this.edges);
	}
	
	public Collection<Edge<W, D>> getEdgeList() {
		return (this.edges.values());
	}
	
	public int getId() {
		return (this.id);
	}
	
	public void deleteEdge(int id) {
		this.edges.remove(id);
	}
	
	public void addEdge(Edge<W, D> edge) {
		this.edges.put(edge.getId(), edge);
	}
	
	public D getData() {
		return (this.data);
	}
	
	public void setData(D data) {
		this.data = data;
	}
	
	@Override
	public int hashCode() {
		return (Integer.hashCode(this.id));
	}
	
	@Override
	public String toString() {
		if(this.edges.size() == 0) {
			return ("[" + this.id + ": " + this.data + "]{}");
		}
		StringBuffer out = new StringBuffer("[" + this.id + ": " + this.data + "]{");
		for(Edge<W, D> e : this.getEdgeList()) {
			out.append(e.toString() + ", ");
		}
		out.replace(out.length() - 2, out.length() - 1, "}");
		return (out.toString());
	}
	
	public LinkedList<Node<W, D>> getNeighbors() {
		LinkedList<Node<W, D>> out = new LinkedList<>();
		
		for(Edge<W, D> e : this.getEdgeList()) {
			Node<W, D> n = e.traverse(this);
			if(n != null && !out.contains(n)) {
				out.add(n);
			}
		}
		return (out);
	}
	
	public LinkedList<Node<W, D>> getUndirectedNeighbors() {
		LinkedList<Node<W, D>> out = new LinkedList<>();
		
		for(Edge<W, D> e : this.getEdgeList()) {
			Node<W, D> n = e.traverseUndirected(this);
			if(n != null && !out.contains(n)) {
				out.add(n);
			}
		}
		return (out);
	}
	
	public int getNumEdges() {
		return (this.edges.size());
	}
}
