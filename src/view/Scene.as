package view
{
	import event.SceneEvent;
	
	import face2wind.customUIComponents.Message;
	import face2wind.event.ParamEvent;
	import face2wind.view.BaseSprite;
	
	import findPath.AStarPathFinder;
	
	import flash.display.Graphics;
	import flash.display.Shape;
	import flash.events.Event;
	import flash.geom.Point;
	
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
		private var elementLayer:BaseSprite;
		private var pathLayer:BaseSprite;
		
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
			elementLayer = new BaseSprite();
			addChild(elementLayer);
			pathLayer = new BaseSprite();
			addChild(pathLayer);
			
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
			for (var i:int = 0; i <= mapData[0].length; i++) {
				g.moveTo(0, i*MapItem.HEIGHT);
				g.lineTo(MapItem.WIDTH*(mapData.length),  i*MapItem.HEIGHT);
			}
			for (var j:int = 0; j <= mapData.length; j++) {
				g.moveTo(j*MapItem.WIDTH, 0);
				g.lineTo(j*MapItem.WIDTH, MapItem.HEIGHT*(mapData[0].length));
			}
			for (var k:int = 0; k < mapData.length; k++) {
				for (var l:int = 0; l < mapData[0].length; l++){
					var mapItem:MapItem = new MapItem();
					mapItem.type = mapData[k][l];
					mapItem.move(k*MapItem.WIDTH, l*MapItem.HEIGHT);
					elementLayer.addChild(mapItem);
				}
			}
			
		}
		
		/**
		 * 测试寻路并把路径画出来 
		 */		
		private function showFindPath():void
		{
			var mapData:Array = manager.getMapData();
			var maxX:int = mapData.length-1;
			var randomY1:int = Math.random()*mapData[0].length1;
			var randomY2:int = Math.random()*mapData[0].length;
			var path:Array = pathFinder.findPath(new Point(0,randomY1), new Point(maxX, randomY2));
			if(null == path){
				Message.show("(0,"+randomY1+") -> ("+maxX+","+randomY2+")没有找到合适路径");
				return;
			}
			pathLayer.removeAllChildren();
			for (var i:int = 0; i < path.length; i++) 
			{
				var p:Point = path[i];
				var pathItem:Shape = new Shape();
				pathItem.graphics.beginFill(0xff00ff,0.5);
				pathItem.graphics.drawRect(1,1,MapItem.WIDTH, MapItem.HEIGHT);
				pathItem.graphics.endFill();
				pathItem.x = p.x*MapItem.WIDTH;
				pathItem.y = p.y*MapItem.HEIGHT;
				pathLayer.addChild(pathItem);
			}
		}
		
		protected function onRestartDemoHandler(event:ParamEvent):void
		{
			clearMaze();
			initMaze();
			
			showFindPath();
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