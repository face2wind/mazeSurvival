package view
{
	import event.SceneEvent;
	
	import face2wind.event.ParamEvent;
	import face2wind.view.BaseSprite;
	
	import findPath.AStarPathFinder;
	
	import flash.display.Graphics;
	import flash.display.Shape;
	import flash.events.Event;
	
	import model.SceneManager;
	
	/**
	 * 场景
	 * @author face2wind
	 */
	public class Scene extends BaseSprite
	{
		/**
		 * 网格 
		 */		
		private var meshShape:Shape;
		
		private var manager:SceneManager;
		private var pathFinder:AStarPathFinder;
		
		public function Scene()
		{
			super();
		}
		
		/**
		 * 此函数是视图的内容初始化函数<br/>对父类的覆盖 
		 */		
		protected override function createChildren():void
		{
			super.createChildren();
			meshShape = new Shape();
			addChild(meshShape);
			
			manager = SceneManager.getInstance();
			pathFinder = new AStarPathFinder();
		}
		
		/**
		 * 清空地图 
		 */		
		private function clearMaze():void
		{
			meshShape.graphics.clear();
			
		}
		
		/**
		 * 初始化地图 
		 */		
		private function initMaze():void
		{
			var mapData:Array = manager.getMapData();
			if(null == mapData || 0 == mapData.length)
				return;
			
			pathFinder.setMapData(mapData);
			var g:Graphics = meshShape.graphics;
			g.lineStyle(1,0xff00ff);
			g.moveTo(0,0); // 画出地图边框
			g.lineTo(0, MapItem.HEIGHT*(mapData[0].length-1));
			g.lineTo(MapItem.WIDTH*(mapData.length-1), MapItem.HEIGHT*(mapData[0].length-1));
			g.lineTo(MapItem.WIDTH*(mapData.length-1), 0);
			g.lineTo(0, 0);
			for (var i:int = 0; i < mapData.length; i++) {
				g.moveTo(0, i*MapItem.HEIGHT);
				g.lineTo(MapItem.WIDTH*(mapData.length-1),  i*MapItem.HEIGHT);
			}
			for (var j:int = 0; j < mapData[0].length; j++) {
				g.moveTo(j*MapItem.WIDTH, 0);
				g.lineTo(j*MapItem.WIDTH, MapItem.HEIGHT*(mapData[0].length-1));
			}
		}
		
		protected function onRestartDemoHandler(event:ParamEvent):void
		{
			clearMaze();
			initMaze();
		}
		
		/**
		 * [继承] 恢复资源
		 * 
		 */		
		public override function resume():void
		{
			super.resume();
			manager.addEventListener(SceneEvent.RESTART_DEMO, onRestartDemoHandler);
		}
		
		/**
		 * [继承] 释放资源
		 * 
		 */		
		public override function dispose():void
		{
			super.dispose();
			manager.removeEventListener(SceneEvent.RESTART_DEMO, onRestartDemoHandler);
		}
	}
}