package view
{
	import face2wind.view.BaseSprite;
	
	import model.MapDataType;
	
	/**
	 * 地图基础元素
	 * @author face2wind
	 */
	public class MapItem extends BaseSprite
	{
		/**
		 *  地图元素固定宽度
		 */		
		public static const WIDTH:int = 20;
		/**
		 *  地图元素固定高度
		 */		
		public static const HEIGHT:int =20;

		/**
		 * 地图元素类型
		 */
		private var _type:int;
		private var typeChange:Boolean;
		/**
		 * 地图元素类型
		 */
		public function get type():int
		{
			return _type;
		}
		/**
		 * 地图元素类型
		 */
		public function set type(value:int):void
		{
			if(_type == value)
				return;
			
			_type = value;
			typeChange = true;
			propertyChange();
		}
		
		public function MapItem()
		{
			super();
		}
		
		protected override function update():void
		{
			super.update();
			if(typeChange){
				typeChange = false;
				graphics.clear();
				if(type == MapDataType.OBSTACLE){
					graphics.beginFill(0x0fff00);
					graphics.drawRect(1,1, MapItem.WIDTH-1, MapItem.HEIGHT-1);
					graphics.endFill();
				}
			}
		}
	}
}