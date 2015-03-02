package view
{
	import model.vo.LivingThingVo;
	import model.vo.MonsterVo;

	/**
	 * 怪物
	 * @author face2wind
	 */
	public class Monster extends LivingThing
	{
		public function Monster()
		{
			super();
			_speed = 5;
		}
		
		/**
		 * 此函数是视图的内容初始化函数<br/>对父类的覆盖 
		 * 
		 */		
		protected override function createChildren():void
		{
			super.createChildren();
			graphics.beginFill(0x00ff0f);
			graphics.drawCircle(MapItem.WIDTH/2,MapItem.HEIGHT/2, MapItem.WIDTH/2);
			graphics.endFill();
		}
		
		public function get monsterVo():MonsterVo
		{
			return livingVo as MonsterVo;
		}
		public function set monsterVo(value:MonsterVo):void
		{
			livingVo = value as LivingThingVo;
		}
	}
}