package graph;

public class Edge<W, D> {
	private W weight;
	private Node<W, D> start, end;
	private boolean isDirected;
	private int id;
	
	
	public Edge(Node<W, D> start, Node<W, D> end, int id, boolean directed) { 
		this.start = start;
		this.end = end;
		this.weight = null;
		this.isDirected = directed;
		this.id = id;
		this.start.addEdge(this);
		this.end.addEdge(this);
	}
	
	public Edge(Node<W, D> start, Node<W, D> end, int id, W weight, boolean directed) {
		this.start = start;
		this.end = end;
		this.weight = weight;
		this.isDirected = directed;
		this.id = id;
		this.start.addEdge(this);
		this.end.addEdge(this);
	}
	
	public Node<W, D> getStart() {
		return (this.start);
	}
	
	public Node<W, D> getEnd() {
		return (this.end);
	}
	
	public Node<W, D> traverse(Node<W, D> start) {
		if(start.equals(this.start)) {
			return (this.end);
		} else if(!this.isDirected && start.equals(this.end)) {
			return (this.start);
		} else {
			return (null);
		}
	}
	
	public Node<W, D> traverse(int start) {
		if(start == this.start.getId()) {
			return (this.end);
		} else if(!this.isDirected && start == this.end.getId()) {
			return (this.start);
		} else {
			return (null);
		}
	}
	
	public Node<W, D> traverseUndirected(Node<W, D> start) {
		if(start.equals(this.start)) {
			return (this.end);
		} else if(start.equals(this.end)) {
			return (this.start);
		} else {
			return (null);
		}
	}
	
	public Node<W, D> traverseUndirected(int start) {
		if(start == this.start.getId()) {
			return (this.end);
		} else if(start == this.end.getId()) {
			return (this.start);
		} else {
			return (null);
		}
	}
	
	public W getWeight() {
		return (this.weight);
	}
	
	public void setWeight(W weight) {
		this.weight = weight;
	}
	
	public int getId() {
		return (this.id);
	}
	
	public boolean isDirected() {
		return (this.isDirected);
	}
	
	@Override
	public int hashCode() {
		return (Integer.hashCode(this.id));
	}
	
	@Override
	public String toString() {
		StringBuffer out = new StringBuffer("(" + this.start.getId() + " -");
		if(this.isDirected) {
			out.append("> ");
		} else {
			out.append(" ");
		}
		out.append(this.end.getId() + " " + this.getWeight() + ")");
		return (out.toString());
	}
}
