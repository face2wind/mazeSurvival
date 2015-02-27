package view
{
	import face2wind.manager.item.IRender;
	
	import model.MovingDirection;
	import model.vo.LivingThingVo;
	import model.vo.PlayerVo;

	/**
	 * 玩家
	 * @author face2wind
	 */
	public class Player extends LivingThing
	{
		public function Player()
		{
			super();
			_speed = 4;
		}
		
		/**
		 * 此函数是视图的内容初始化函数<br/>对父类的覆盖 
		 * 
		 */		
		protected override function createChildren():void
		{
			super.createChildren();
			graphics.beginFill(0x00ffff);
			graphics.drawCircle(MapItem.WIDTH/2,MapItem.HEIGHT/2, MapItem.WIDTH/2);
			graphics.endFill();
		}
		
		public function get playerVo():PlayerVo
		{
			return livingVo as PlayerVo;
		}
		public function set playerVo(value:PlayerVo):void
		{
			livingVo = value as LivingThingVo;
		}
	}
}