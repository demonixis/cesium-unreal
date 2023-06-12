// Copyright 2020-2023 CesiumGS, Inc. and Contributors

#pragma once

#include "Components/ActorComponent.h"
#include "UObject/ObjectMacros.h"
#include "CesiumSubLevelComponent.generated.h"

class ACesiumGeoreference;
class ALevelInstance;
class UCesiumSubLevelSwitcherComponent;

UCLASS(ClassGroup = (Cesium), meta = (BlueprintSpawnableComponent))
class CESIUMRUNTIME_API UCesiumSubLevelComponent : public UActorComponent {
  GENERATED_BODY()

public:
  /** @copydoc UCesiumSubLevelComponent::OriginLongitude */
  UFUNCTION(BlueprintPure, Category = "Cesium")
  double GetOriginLongitude() const;
  /** @copydoc UCesiumSubLevelComponent::OriginLongitude */
  UFUNCTION(BlueprintCallable, Category = "Cesium")
  void SetOriginLongitude(double value);

  /** @copydoc UCesiumSubLevelComponent::OriginLatitude */
  UFUNCTION(BlueprintPure, Category = "Cesium")
  double GetOriginLatitude() const;
  /** @copydoc UCesiumSubLevelComponent::OriginLatitude */
  UFUNCTION(BlueprintCallable, Category = "Cesium")
  void SetOriginLatitude(double value);

  /** @copydoc UCesiumSubLevelComponent::OriginHeight */
  UFUNCTION(BlueprintPure, Category = "Cesium")
  double GetOriginHeight() const;
  /** @copydoc UCesiumSubLevelComponent::OriginHeight */
  UFUNCTION(BlueprintCallable, Category = "Cesium")
  void SetOriginHeight(double value);

  /** @copydoc UCesiumSubLevelComponent::LoadRadius */
  UFUNCTION(BlueprintPure, Category = "Cesium")
  double GetLoadRadius() const;
  /** @copydoc UCesiumSubLevelComponent::LoadRadius */
  UFUNCTION(BlueprintCallable, Category = "Cesium")
  void SetLoadRadius(double value);

  /** @copydoc UCesiumSubLevelComponent::Georeference */
  UFUNCTION(BlueprintPure, Category = "Cesium")
  ACesiumGeoreference* GetGeoreference() const;
  /** @copydoc UCesiumSubLevelComponent::Georeference */
  UFUNCTION(BlueprintCallable, Category = "Cesium")
  void SetGeoreference(ACesiumGeoreference* NewGeoreference);

  /**
   * Gets the resolved georeference, just like calling the ResolveGeoreference
   * property, except that it will return null if a georeference has not yet
   * been resolved.
   */
  UFUNCTION(BlueprintPure, Category = "Cesium")
  ACesiumGeoreference* GetResolvedGeoreference() const;

  /**
   * Resolves the Cesium Georeference to use with this components. Returns
   * the value of the Georeference property if it is set. Otherwise, finds a
   * Georeference in the World and returns it, creating it if necessary. The
   * resolved Georeference is cached so subsequent calls to this function will
   * return the same instance.
   */
  UFUNCTION(BlueprintCallable, Category = "Cesium")
  ACesiumGeoreference* ResolveGeoreference();

  /**
   * Invalidates the cached resolved georeference, unsubscribing from it and
   * setting it to null. The next time ResolveGeoreference is called, the
   * Georeference will be re-resolved and re-subscribed.
   */
  UFUNCTION(BlueprintCallable, Category = "Cesium")
  void InvalidateResolvedGeoreference();

  /**
   * Sets the longitude (X), latitude (Y), and height (Z) of this sub-level's
   * georeference origin. When this sub-level is active, the CesiumGeoreference
   * will adopt this origin. Longitude and latitude are in degrees. Height is in
   * meters above the WGS84 ellipsoid, which should not be confused with meters
   * above Mean Sea Level.
   */
  UFUNCTION(BlueprintCallable, Category = "Cesium")
  void SetOriginLongitudeLatitudeHeight(const FVector& longitudeLatitudeHeight);

  /**
   * If this sub-level is currently the active one, this method will copy its
   * origin to the georeference's origin. Otherwise, it does nothing.
   */
  void UpdateGeoreferenceIfSubLevelIsActive();

  virtual void BeginDestroy() override;

protected:
  virtual void BeginPlay() override;

  /**
   * Called when a component is registered. This can be viewed as "enabling"
   * this Component on the Actor to which it is attached.
   *
   * In the Editor, this is called in a many different situations, such as on
   * changes to properties.
   */
  virtual void OnRegister() override;

  /**
   * Called when a component is unregistered. This can be viewed as
   * "disabling" this Component on the Actor to which it is attached.
   *
   * In the Editor, this is called in a many different situations, such as on
   * changes to properties.
   */
  virtual void OnUnregister() override;

private:
  /**
   * The latitude of the georeference origin for this sublevel in degrees, in
   * the range [-90, 90]. When this sub-level is active, the CesiumGeoreference
   * will adopt this origin.
   */
  UPROPERTY(
      EditAnywhere,
      BlueprintReadWrite,
      Category = "Cesium",
      BlueprintGetter = GetOriginLongitude,
      BlueprintSetter = SetOriginLongitude,
      meta = (ClampMin = -90.0, ClampMax = 90.0, AllowPrivateAccess = true))
  double OriginLatitude = 39.736401;

  /**
   * The longitude of the georeference origin for this sublevel in degrees, in
   * the range [-180, 180]. When this sub-level is active, the
   * CesiumGeoreference will adopt this origin.
   */
  UPROPERTY(
      EditAnywhere,
      BlueprintReadWrite,
      Category = "Cesium",
      BlueprintGetter = GetOriginLatitude,
      BlueprintSetter = SetOriginLatitude,
      meta = (ClampMin = -180.0, ClampMax = 180.0, AllowPrivateAccess = true))
  double OriginLongitude = -105.25737;

  /**
   * The height of the georeference origin for this sublevel in meters above the
   * WGS84 ellipsoid. This height should not be confused with a height above
   * Mean Sea Level. When this sub-level is active, the CesiumGeoreference will
   * adopt this origin.
   */
  UPROPERTY(
      EditAnywhere,
      BlueprintReadWrite,
      Category = "Cesium",
      BlueprintGetter = GetOriginHeight,
      BlueprintSetter = SetOriginHeight,
      meta = (AllowPrivateAccess = true))
  double OriginHeight = 2250.0;

  /**
   * How close to the sublevel local origin the camera needs to be to load the
   * level.
   */
  UPROPERTY(
      EditAnywhere,
      BlueprintReadWrite,
      Category = "Cesium",
      BlueprintGetter = GetLoadRadius,
      BlueprintSetter = SetLoadRadius,
      meta = (ClampMin = 0.0, AllowPrivateAccess = true))
  double LoadRadius = 1000.0;

  /**
   * The designated georeference actor controlling how the actor's
   * coordinate system relates to the coordinate system in this Unreal Engine
   * level.
   *
   * If this is null, the sub-level will find and use the first Georeference
   * Actor in the level, or create one if necessary. To get the active/effective
   * Georeference from Blueprints or C++, use ResolvedGeoreference instead.
   */
  UPROPERTY(
      EditAnywhere,
      BlueprintReadWrite,
      BlueprintGetter = GetGeoreference,
      BlueprintSetter = SetGeoreference,
      Category = "Cesium",
      Meta = (AllowPrivateAccess))
  ACesiumGeoreference* Georeference;

  /**
   * The resolved georeference used by this sub-level. This is not serialized
   * because it may point to a Georeference in the PersistentLevel while this
   * Actor is in a sublevel. If the Georeference property is specified,
   * however then this property will have the same value.
   *
   * This property will be null before ResolveGeoreference is called.
   */
  UPROPERTY(
      Transient,
      BlueprintReadOnly,
      Category = "Cesium",
      Meta = (AllowPrivateAccess))
  ACesiumGeoreference* ResolvedGeoreference = nullptr;

  /**
   * Gets the sub-level switch component with which this sub-level is
   * associated. Calling this method will call ResolveGeoreference to resolve
   * the georeference, if it's not already resolved.
   */
  UCesiumSubLevelSwitcherComponent* _getSwitcher() noexcept;

  /**
   * Gets the Level Instance Actor to which this component is attached. If this
   * component is not attached to a Level Instance Actor, this method logs a
   * warning and returns nullptr.
   */
  ALevelInstance* _getLevelInstance() const noexcept;
};
