package model
{
	/**
	 * 地图元素类型
	 * @author face2wind
	 */
	public class MapDataType
	{
		public function MapDataType()
		{
		}
		
		/**
		 * 普通可行走地表 
		 */		
		public static const GROUND:int = 0;
		
		/**
		 * 障碍物
		 */		
		public static const OBSTACLE:int = 1;
		
		/**
		 * 获取一个随机地图元素类型 
		 * @return 
		 */		
		public static function getRandomType():int
		{
			if(0.8 > Math.random())
				return GROUND;
			else
				return OBSTACLE;
		}
	}
}