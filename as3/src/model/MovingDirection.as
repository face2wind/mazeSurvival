package model
{
	/**
	 * 地图中的移动方向
	 * @author face2wind
	 */
	public class MovingDirection
	{
		public function MovingDirection()
		{
		}
		public static const STOP:int = 1;
		public static const TOP:int = 2;
		public static const BOTTOM:int = 3;
		public static const LEFT:int = 4;
		public static const RIGHT:int = 5;
		
		public static const TOP_RIGHT:int = 6;
		public static const TOP_LEFT:int = 7;
		public static const BOTTOM_RIGHT:int = 8;
		public static const BOTTOM_LEFT:int = 9;
	}
}