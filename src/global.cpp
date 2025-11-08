#include <sepia/comm2/global.h>

#include <yaml-cpp/yaml.h>

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

namespace
{
    constexpr const char* defaultConfigFile = "sepia.yaml";

    std::string getEnvVar( const char* name )
    {
        const char* value = std::getenv( name );
        return value ? std::string( value ) : std::string();
    }

    std::string resolveConfigPath()
    {
        const std::string configuredPath = getEnvVar( "SEPIA_CONFIG" );
        if( !configuredPath.empty() )
        {
            return configuredPath;
        }
        return defaultConfigFile;
    }

    bool fileExists( const std::string& a_path )
    {
        std::error_code ec;
        return std::filesystem::exists( a_path, ec );
    }
}

namespace sepia::comm2
{
    std::shared_ptr< Global > Global::instance()
    {
        static std::shared_ptr< Global > shared = std::shared_ptr< Global >( new Global() ); // cannot use make_shared due to private constructor
        return shared;
    }

    Global::Config Global::loadConfiguration()
    {
        Config cfg;
        const std::string configPath = resolveConfigPath();
        const bool usingDefaultPath = ( configPath == defaultConfigFile );

        if( configPath.empty() )
        {
            return cfg;
        }

        if( !fileExists( configPath ) )
        {
            if( !usingDefaultPath )
            {
                std::cerr << "SEPIA_CONFIG points to missing file: " << configPath << std::endl;
            }
            return cfg;
        }

        try
        {
            const YAML::Node root = YAML::LoadFile( configPath );

            if( const YAML::Node publisher = root[ "publisher_url" ] )
            {
                cfg.publisherUrl = publisher.as< std::string >( cfg.publisherUrl );
            }

            if( const YAML::Node subscriber = root[ "subscriber_url" ] )
            {
                cfg.subscriberUrl = subscriber.as< std::string >( cfg.subscriberUrl );
            }

            if( const YAML::Node threads = root[ "context_threads" ] )
            {
                cfg.contextThreads = std::max( 1, threads.as< int >( cfg.contextThreads ) );
            }
        }
        catch( const YAML::Exception& ex )
        {
            std::cerr << "Unable to parse configuration file '" << configPath << "': " << ex.what() << std::endl;
        }

        return cfg;
    }

    Global::Global()
        : m_config( loadConfiguration() )
        , m_context( m_config.contextThreads )
    {
    }

    Global::~Global()
    {
    }

    zmq::context_t& Global::getContext()
    {
        return m_context;
    }

    std::string Global::getSubscriberUrl()
    {
        return m_config.subscriberUrl;
    }

    std::string Global::getPublisherUrl()
    {
        return m_config.publisherUrl;
    }
}
