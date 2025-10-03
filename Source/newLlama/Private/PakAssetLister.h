#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "IPlatformFilePak.h"
#include "Misc/Paths.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/AssetManager.h"

/**
 * 用于列出Pak文件中所有资源的工具类
 */
class NEWLLAMA_API FPakAssetLister
{
public:
    /**
     * 打印指定Pak文件中的所有资源
     * @param PakFilePath Pak文件的路径
     */
    static void ListAssetsInPakFile(const FString& PakFilePath);
    
    /**
     * 打印所有已加载Pak文件中的资源
     */
    static void ListAllLoadedPakAssets();
    
    /**
     * 通过AssetRegistry打印所有已注册的资源
     */
    static void ListAllRegisteredAssets();
    
    /**
     * 搜索特定名称的资源
     * @param AssetName 要搜索的资源名称
     */
    static void SearchAssetByName(const FString& AssetName);
    
    /**
     * 打印项目中所有的Pak文件
     */
    static void ListAllPakFiles();

private:
    /**
     * 递归遍历Pak文件系统
     */
    static void RecursiveListPakContents(IPlatformFile& PakPlatformFile, const FString& Directory, int32 Depth = 0);
    
    /**
     * 格式化输出辅助函数
     */
    static FString GetIndentString(int32 Depth);
};