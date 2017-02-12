package findPath
{
	import flash.geom.Point;

	/**
	 * 地图寻路接口
	 * @author face2wind
	 */
	public interface IPathFinder
	{
		/**
		 * 地图二维数组，里面的值只能是（0 - 可走区，1 - 障碍区） 
		 * @param data
		 */		
		function setMapData(data:Array):void;
		
		/**
		 * 查找路径 
		 * @param startPoint
		 * @param endPoint
		 * @return 返回寻好的路径点数组，包含所有路径上的点
		 */		
		function findPath(startPoint:Point, endPoint:Point):Array;
	}
}