#include "bvh.h"

Bvh::Bvh()
    : rootJoint(NULL)
{
    motionData.data = 0;
}

void deleteJoint(JOINT* joint)
{
    if( joint == NULL )
        return;
    for(JOINT* child : joint->children)
        deleteJoint(child);
    if( joint->channels_order != NULL )
        delete joint->channels_order;
    delete joint;
}

Bvh::~Bvh()
{
    ofLog() << "Bvh destructor";
    deleteJoint(rootJoint);
    if( motionData.data != NULL )
        delete[] motionData.data;
}

void Bvh::load(const std::string& filename)
{
    std::fstream file;
    string p = filename;
    file.open(p.c_str(), std::ios_base::in);

    if( file.is_open() )
    {
        std::string line;

        while( file.good() )
        {
            file >> line;
            ofLog() << line;
            if( trim(line) == "HIERARCHY" )
                loadHierarchy(file);
            break;
        }

        file.close();
    }
}

void Bvh::printJoint(const JOINT* const joint) const
{
    ofLog() << "print joint" << joint->name << joint->channel_start;

    for(std::vector<JOINT*>::const_iterator ct = joint->children.begin();
        ct != joint->children.end(); ++ct)
    {
        JOINT* _tmp = *ct;

        if( _tmp->children.size() > 0 )
            printJoint(_tmp);
    }
}

void moveJoint(JOINT* joint, MOTION* motionData, int frame_starts_index)
{
    int start_index = frame_starts_index + joint->channel_start;
    joint->matrix = glm::translate(glm::mat4(1.0),
                                   glm::vec3(joint->offset.x,
                                             joint->offset.y,
                                             joint->offset.z));
    for(int i = 0; i < joint->num_channels; i++)
    {
        const short& channel = joint->channels_order[i];
        float value = motionData->data[start_index + i];
        if( channel & Xposition )
        {
            joint->matrix = glm::translate(joint->matrix, glm::vec3(value, 0, 0));
        }
        if( channel & Yposition )
        {
            joint->matrix = glm::translate(joint->matrix, glm::vec3(0, value, 0));
        }
        if( channel & Zposition )
        {
            joint->matrix = glm::translate(joint->matrix, glm::vec3(0, 0, value));
        }

        if( channel & Xrotation )
        {
            joint->matrix = glm::rotate(joint->matrix, value, glm::vec3(1, 0, 0));
        }
        if( channel & Yrotation )
        {
            joint->matrix = glm::rotate(joint->matrix, value, glm::vec3(0, 1, 0));
        }
        if( channel & Zrotation )
        {
            joint->matrix = glm::rotate(joint->matrix, value, glm::vec3(0, 0, 1));
        }
    }
    if( joint->parent != NULL )
        joint->matrix = joint->parent->matrix * joint->matrix;
    for(auto& child : joint->children)
        moveJoint(child, motionData, frame_starts_index);
}

void Bvh::moveTo(unsigned frame)
{
    unsigned start_index = frame * motionData.num_motion_channels;
    moveJoint(rootJoint, &motionData, start_index);
}

void Bvh::testOutput() const
{
    if( rootJoint == 0 )
        return;
    ofLog() << "output";
    printJoint(rootJoint);
    ofLog() << "num frames: " << motionData.num_frames;
    ofLog() << "num motion channels: " << motionData.num_motion_channels;
    int num_frames = motionData.num_frames;
    int num_channels = motionData.num_motion_channels;
}

void Bvh::loadHierarchy(std::istream& stream)
{
    std::string tmp;
    while( stream.good() )
    {
        stream >> tmp;
        ofLog() << tmp;
        if( trim(tmp) == "ROOT" )
            rootJoint = loadJoint(stream);
        else if( trim(tmp) == "MOTION" )
            loadMotion(stream);
    }
}

void Bvh::loadMotion(std::istream& stream)
{
    std::string tmp;
    while( stream.good() )
    {
        stream >> tmp;
        if( trim(tmp) == "Frames:" )
        {
            stream >> motionData.num_frames;
        }
        else if( trim(tmp) == "Frame" )
        {
            float frame_time;
            stream >> tmp >> frame_time;

            int num_frames   = motionData.num_frames;
            int num_channels = motionData.num_motion_channels;

            ofLog() << "num frames:" << num_frames;
            ofLog() << "num channels:" << num_channels;

            motionData.data = new float[num_frames * num_channels];

            for( int frame = 0; frame < num_frames; frame++ )
            {
                for( int channel = 0; channel < num_channels; channel++)
                {
                    float x;
                    std::stringstream ss;
                    stream >> tmp;
                    ss << tmp;
                    ss >> x;

                    int index = frame * num_channels + channel;
                    motionData.data[index] = x;
                }
            }
        }
    }
}

JOINT* Bvh::loadJoint(std::istream& stream, JOINT* parent)
{
    JOINT* joint = new JOINT;
    joint->parent = parent;
    std::string* name = new std::string;
    stream >> *name;
    joint->name = name->c_str();
    std::string tmp;
    joint->matrix = glm::mat4(1.0);

    static int _channel_start = 0;
    unsigned channel_order_index = 0;
    while( stream.good() )
    {
        stream >> tmp;
        tmp = trim(tmp);
        char c = tmp.at(0);
        if( c == 'X' || c == 'Y' || c == 'Z' )
        {
            if( tmp == "Xposition" )
            {
                joint->channels_order[channel_order_index++] = Xposition;
            }
            if( tmp == "Yposition" )
            {
                joint->channels_order[channel_order_index++] = Yposition;
            }
            if( tmp == "Zposition" )
            {
                joint->channels_order[channel_order_index++] = Zposition;
            }

            if( tmp == "Xrotation" )
            {
                joint->channels_order[channel_order_index++] = Xrotation;
            }
            if( tmp == "Yrotation" )
            {
                joint->channels_order[channel_order_index++] = Yrotation;
            }
            if( tmp == "Zrotation" )
            {
                joint->channels_order[channel_order_index++] = Zrotation;
            }
        }

        if( tmp == "OFFSET" )
        {
            stream  >> joint->offset.x
                    >> joint->offset.y
                    >> joint->offset.z;
        }
        else if( tmp == "CHANNELS" )
        {
            stream >> joint->num_channels;
            motionData.num_motion_channels += joint->num_channels;
            joint->channel_start = _channel_start;
            _channel_start += joint->num_channels;
            joint->channels_order = new short[joint->num_channels];

        }
        else if( tmp == "JOINT" )
        {
            JOINT* tmp_joint = loadJoint(stream, joint);

            tmp_joint->parent = joint;
            joint->children.push_back(tmp_joint);
        }
        else if( tmp == "End" )
        {
            stream >> tmp >> tmp;

            JOINT* tmp_joint = new JOINT;

            tmp_joint->parent = joint;
            tmp_joint->num_channels = 0;
            tmp_joint->name = "EndSite";
            joint->children.push_back(tmp_joint);

            stream >> tmp;
            if( tmp == "OFFSET" )
                stream >> tmp_joint->offset.x
                       >> tmp_joint->offset.y
                       >> tmp_joint->offset.z;
            stream >> tmp;
        }
        else if( tmp == "}" )
            return joint;

    }
}













