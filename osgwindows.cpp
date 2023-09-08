/* OpenSceneGraph example, osgwindows.
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*  THE SOFTWARE.
*/

#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>
#include <osgUtil/SmoothingVisitor>
#include <osgUtil/TangentSpaceGenerator>

#include <iostream>
#include <vector>

#include <osg/TexGen>
//生成TBN
class CreateTangentSpace : public osg::NodeVisitor
{
public:
	CreateTangentSpace() : NodeVisitor(NodeVisitor::TRAVERSE_ALL_CHILDREN), tsg(new osgUtil::TangentSpaceGenerator) {}
	virtual void apply(osg::Geode& geode)
	{
		for (unsigned int i = 0; i < geode.getNumDrawables(); ++i)
		{
			osg::Geometry* geo = dynamic_cast<osg::Geometry*>(geode.getDrawable(i));
			if (geo != NULL)
			{
				tsg->generate(geo, 0);
				geo->setTexCoordArray(1, tsg->getTangentArray());
			}
		}
		traverse(geode);
	}
private:
	osg::ref_ptr<osgUtil::TangentSpaceGenerator> tsg;
};


class Water
{
public:
	Water();
	~Water();

	virtual void slotRightHandle();

	void drawWater(std::vector< osg::Vec3d>& pos, osgViewer::Viewer* pViewer);
	osg::Texture2D* creatText2D(const std::string& strFile);

	osg::Node* getNode() { return m_pWater; }

	osg::ref_ptr<osg::Geometry> createQuad(bool texCoord=true);

	void initFFtPrograme(osg::StateSet* pWaterGeodeStateSet);

	void initNormalPrograme(osg::StateSet* pWaterGeodeStateSet, osgViewer::Viewer* pViewer,bool );

	void initBasePrograme(osg::StateSet* pWaterGeodeStateSet);

	void generateTexCoord(osg::StateSet* stateset);

private:
	osg::ref_ptr<osg::Vec3dArray> m_vecPoints;
	osg::Group* m_pWater;
	char* m_waterFrag;
	char* m_waterVert;
};

Water::Water()
{
	m_pWater = nullptr;
	m_vecPoints = new osg::Vec3dArray;
	m_vecPoints->clear();
}

Water::~Water()
{

}


void Water::slotRightHandle()
{
	m_pWater = nullptr;
}

//创建一个四边形节点
osg::ref_ptr<osg::Geometry> Water::createQuad(bool texCoord)
{
	//创建一个几何体对象
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

	//顶点数组，注意顶点的添加顺序是逆时针的
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
	v->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
	v->push_back(osg::Vec3(1.0f, 0.0f, 0.0f));
	v->push_back(osg::Vec3(1.0f, 0.0f, 1.0f));
	v->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
	geom->setVertexArray(v.get());

	if (texCoord)
	{
		//纹理坐标
		osg::ref_ptr<osg::Vec2Array> vt = new osg::Vec2Array();
		vt->push_back(osg::Vec2(0.0f, 0.0f));
		vt->push_back(osg::Vec2(1.0f, 0.0f));
		vt->push_back(osg::Vec2(1.0f, 1.0f));
		vt->push_back(osg::Vec2(0.0f, 1.0f));
		geom->setTexCoordArray(0, vt.get());
	}

	//创建颜色数组
	/*osg::ref_ptr<osg::Vec4Array> vc = new osg::Vec4Array();
	vc->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	vc->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vc->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	vc->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	geom->setColorArray(vc.get());
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);*/

	//添加图元，绘图基元为四边形
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

	return geom.get();
}

void Water::initFFtPrograme(osg::StateSet* pWaterGeodeStateSet)
{
	
	//纹理
	pWaterGeodeStateSet->setTextureAttributeAndModes(0,
		creatText2D("D:/workSpace/test/texture/water2.jpg"));
	pWaterGeodeStateSet->setTextureAttributeAndModes(1,
		creatText2D("D:/workSpace/test/texture/flowmap.png"));


	pWaterGeodeStateSet->addUniform(new osg::Uniform("waterDiffuse", 0));
	pWaterGeodeStateSet->addUniform(new osg::Uniform("waterFlowMap", 1));

	//// shader
	osg::ref_ptr<osg::Shader> pVeter = osg::Shader::readShaderFile(osg::Shader::VERTEX, "D:/workSpace/test/shader/waterFlowMap.vert");
	osg::ref_ptr<osg::Shader> pFrag = osg::Shader::readShaderFile(osg::Shader::FRAGMENT, "D:/workSpace/test/shader/waterFlowMap.frag");

	osg::ref_ptr<osg::Program> pProgram = new osg::Program;
	pProgram->addShader(pVeter);
	pProgram->addShader(pFrag);

	pWaterGeodeStateSet->setAttributeAndModes(pProgram);
}

class EyeUniformCallback : public osg::UniformCallback
{
public:
	EyeUniformCallback(osgViewer::Viewer *viewer): pViewer(viewer) {}
	virtual void operator() (osg::Uniform* uniform, osg::NodeVisitor* nv)
	{
		osg::Vec3f eye;
		osg::Vec3f center;
		osg::Vec3f up;
		pViewer->getCamera()->getViewMatrixAsLookAt(eye,center,up);
		uniform->set(eye);
	}
private:
	osgViewer::Viewer* pViewer;
};

void Water::initNormalPrograme(osg::StateSet* pWaterGeodeStateSet, osgViewer::Viewer* pViewer,bool manulTexCoord)
{
	//纹理
	osg::ref_ptr<osg::Texture2D> pText2D = new osg::Texture2D;
	pText2D->setTextureSize(1024, 1024);
	pText2D->setInternalFormat(GL_RGBA);
	pWaterGeodeStateSet->setTextureAttributeAndModes(0, pText2D);
	pWaterGeodeStateSet->setTextureAttributeAndModes(1,
		creatText2D("D:/workSpace/test/texture/water.jpg"));
	pWaterGeodeStateSet->setTextureAttributeAndModes(2,
		creatText2D("D:/workSpace/test/texture/water_dudv.jpg"));
	pWaterGeodeStateSet->setTextureAttributeAndModes(3,
		creatText2D("D:/workSpace/test/texture/water_nm.jpg"));

	pWaterGeodeStateSet->addUniform(new osg::Uniform("defaultTex", 0));
	pWaterGeodeStateSet->addUniform(new osg::Uniform("reflection", 1));
	pWaterGeodeStateSet->addUniform(new osg::Uniform("refraction", 2));
	pWaterGeodeStateSet->addUniform(new osg::Uniform("normalTex", 3));

	osg::Vec3 eye(0, 0, 0);
	osg::ref_ptr<osg::Uniform> eyeUniform = new osg::Uniform("viewPos",eye);
	eyeUniform->setUpdateCallback(new EyeUniformCallback(pViewer));
	pWaterGeodeStateSet->addUniform(eyeUniform);
	//// shader
	osg::ref_ptr<osg::Shader> pVeter;
	if (manulTexCoord) 
		pVeter = osg::Shader::readShaderFile(osg::Shader::VERTEX, "D:/workSpace/test/shader/water_normal.vert");
	else
		pVeter = osg::Shader::readShaderFile(osg::Shader::VERTEX, "D:/workSpace/test/shader/water_normal_auto_gen_texCoord.vert");
	
	osg::ref_ptr<osg::Shader> pFrag = osg::Shader::readShaderFile(osg::Shader::FRAGMENT, "D:/workSpace/test/shader/water_normal.frag");

	osg::ref_ptr<osg::Program> pProgram = new osg::Program;
	pProgram->addShader(pVeter);
	pProgram->addShader(pFrag);
	pWaterGeodeStateSet->setAttributeAndModes(pProgram);
}

void Water::initBasePrograme(osg::StateSet* pWaterGeodeStateSet)
{
	pWaterGeodeStateSet->setTextureAttributeAndModes(0,
		creatText2D("D:/workSpace/test/texture/sky.jpg"));

	pWaterGeodeStateSet->addUniform(new osg::Uniform("diffuse", 0));
	
	osg::ref_ptr<osg::Shader> pVeter = osg::Shader::readShaderFile(osg::Shader::VERTEX, "D:/workSpace/test/shader/base.vert");
	osg::ref_ptr<osg::Shader> pFrag = osg::Shader::readShaderFile(osg::Shader::FRAGMENT, "D:/workSpace/test/shader/base.frag");

	osg::ref_ptr<osg::Program> pProgram = new osg::Program;
	pProgram->addShader(pVeter);
	pProgram->addShader(pFrag);
	pWaterGeodeStateSet->setAttributeAndModes(pProgram);
}

void Water::generateTexCoord(osg::StateSet* stateset)
{
	osg::TexGen* texgen = new osg::TexGen;
	texgen->setMode(osg::TexGen::OBJECT_LINEAR);
	texgen->setPlane(osg::TexGen::S, osg::Plane(1.0f, 1.0f, 0.0f, 0.0f));
	//texgen->setPlane(osg::TexGen::T, osg::Plane(1.0f, 1.0f, 1.0f, 0.0f));

	stateset->setTextureMode(0, GL_TEXTURE_GEN_S, osg::StateAttribute::ON);
	//stateset->setTextureMode(0, GL_TEXTURE_GEN_T, osg::StateAttribute::ON);
	stateset->setTextureAttribute(0, texgen);
}

void Water::drawWater(std::vector< osg::Vec3d>& postions , osgViewer::Viewer*pViewer)
{
	osg::ref_ptr<osg::Geode> pWaterGeode = new osg::Geode;
	
	bool manulTexCoord = false; //是否手动添加纹理坐标

	osg::ref_ptr<osg::Geometry> geometry = createQuad(manulTexCoord);
	
	osgUtil::SmoothingVisitor::smooth(*(geometry.get())); //生成顶点法线

	pWaterGeode->addDrawable(geometry);
	
	osg::StateSet* pWaterGeodeStateSet = pWaterGeode->getOrCreateStateSet();

	//initFFtPrograme(pWaterGeodeStateSet);
	//initBasePrograme(pWaterGeodeStateSet);
	if(1)
	{
		CreateTangentSpace cts;
		pWaterGeode->accept(cts);
		initNormalPrograme(pWaterGeodeStateSet, pViewer, manulTexCoord);
	}

	if (m_pWater == NULL)
	{
		m_pWater = new osg::Group;
	}
	m_pWater->addChild(pWaterGeode);
	m_pWater->setStateSet(pWaterGeode->getOrCreateStateSet());
}

osg::Texture2D* Water::creatText2D(const std::string& strFile)
{

	osg::ref_ptr<osg::Texture2D> pText2D = new osg::Texture2D;
	pText2D->setImage(osgDB::readImageFile(strFile));
	pText2D->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
	pText2D->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
	pText2D->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
	pText2D->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture::LINEAR);
	return pText2D.release();
}


int main(int argc, char** argv)
{
	osgViewer::Viewer viewer;

	Water water;
	std::vector< osg::Vec3d> postions;
	water.drawWater(postions,&viewer);

	viewer.setSceneData(water.getNode());

	return viewer.run();
}


