package findPath
{
	import face2wind.lib.Debuger;
	
	import flash.geom.Point;
	
	/**
	 * A*寻路
	 * @author face2wind
	 */
	public class AStarPathFinder implements IPathFinder
	{
		public function AStarPathFinder()
		{
		}
		
		private var _mapData:Array;
		
		public function setMapData(data:Array):void
		{
			_mapData = data.concat();
		}
		
		public function findPath(startPoint:Point, endPoint:Point):Array
		{
			if(null == _mapData || 0 == _mapData.length){
				Debuger.show(Debuger.BASE_LIB, "AStarPathFinder.findPath : mapData is null or illegal");
				return null;
			}
			return null;
		}
	}
}