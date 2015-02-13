package view
{
	import face2wind.view.BaseSprite;
	
	/**
	 * 地图基础元素
	 * @author face2wind
	 */
	public class MapItem extends BaseSprite
	{
		/**
		 *  地图元素固定宽度
		 */		
		public static const WIDTH:int = 50;
		/**
		 *  地图元素固定高度
		 */		
		public static const HEIGHT:int = 50;
		
		public function MapItem()
		{
			super();
		}
	}
}