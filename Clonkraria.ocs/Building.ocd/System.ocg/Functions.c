// Returns the number of stuck vertices. (of this)
global func VerticesStuckSemi()
{
	var vertices = 0;
	// Loop through vertices.
	for (var i = -1; i < GetVertexNum(); i++)
		// Solid?
		if (GBackSemiSolid(GetVertex(i, VTX_X), GetVertex(i, VTX_Y)))
			// Count vertices.
			vertices++;
	return vertices;
}